#include <windows.h>
#include <stdio.h>

#include "../game/run_around_game.cpp"

static bool programRunning = false;

static int gameWidth = 1280;
static int gameHeight = 720;
static float targetMSPerFrame = 1000.0f / 60.0f;

BITMAPINFO bitmapInfo = {};
void *backBuffer = 0;

void clearBuffer (unsigned int *pixels) {
    unsigned char value;
    for (int i = 0; i < gameHeight; ++i) {
        value = 0;
        for (int j = 0; j < gameWidth; ++j) {
            //pixels[i * gameWidth + j] = value << 8;
            //++value;
            pixels[i * gameWidth + j] = value;
        }
    }
}

void initBackBuffer () {
    // Create back buffer
    bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
    bitmapInfo.bmiHeader.biWidth = gameWidth;
    bitmapInfo.bmiHeader.biHeight = -gameHeight; // Make the bitmap start from the top left corner
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 32;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;

    backBuffer = malloc(4 * gameWidth * gameHeight);
    if (backBuffer == 0) {
        printf("Failed to get memory for back buffer.\n");
    }

    // Clear to black
    unsigned int *pixels = (unsigned int *)backBuffer;
    clearBuffer(pixels);
}

void drawSceneToWindow (HWND window, HDC deviceContext) {
    RECT windowDimensions;
    GetWindowRect(window, &windowDimensions);
    int windowWidth = windowDimensions.right - windowDimensions.left;
    int windowHeight = windowDimensions.bottom - windowDimensions.top;

    StretchDIBits(deviceContext, 0 , 0, windowWidth, windowHeight, 0, 0, gameWidth, gameHeight, backBuffer, 
                  &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK windowCallback (HWND window, unsigned int message, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    // TODO(ebuchholz): Determine whether WM_DESTROY is necessary
    switch (message) {
    case WM_CLOSE:
        programRunning = false;
        break;
    default:
        result = DefWindowProcA(window, message, wParam, lParam);
        break;
    }
    return result;
}

void drawLine (render_horizontal_line_command *lineCommand) {
    unsigned int *pixels = (unsigned int *)backBuffer;

    char lineNum = lineCommand->lineNum;
    if (lineNum > 0 && lineNum < gameHeight) {
        for (int x = 0; x < gameWidth; ++x) {
            pixels[lineNum * gameWidth + x] = lineCommand->color;
        }
    }
}

void drawRectangle (render_rectangle_command *rectCommand) {
    unsigned int *pixels = (unsigned int *)backBuffer;

    int startX = rectCommand->x;
    int endX = rectCommand->x + rectCommand->width;
    int startY = rectCommand->y;
    int endY = rectCommand->y + rectCommand->height;

    if (startX < 0) { startX = 0; }
    if (startX > gameWidth - 1) { startX = gameWidth - 1; }
    if (endX < 0) { endX = 0; }
    if (endX > gameWidth - 1) { endX = gameWidth - 1; }
    if (startY < 0) { startY = 0; }
    if (startY > gameHeight - 1) { startY = gameHeight - 1; }
    if (endY < 0) { endY = 0; }
    if (endY > gameHeight - 1) { endY = gameHeight - 1; }

    for (int x = startX; x <= endX; ++x) {
        for (int y = startY; y <= endY; ++y) {
            pixels[y * gameWidth + x] = rectCommand->color;
        }
    }
}

int WINAPI WinMain (HINSTANCE instance, HINSTANCE prevInstance, LPSTR commandLine, int showCode) {
    LARGE_INTEGER perfCountFrequencyResult;
    QueryPerformanceFrequency(&perfCountFrequencyResult);
    long long perfCountFrequency = perfCountFrequencyResult.QuadPart;

    WNDCLASSA windowClass = {};
    windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);
    windowClass.hInstance = instance;
    windowClass.lpfnWndProc = windowCallback;
    windowClass.lpszClassName = "RunAroundGameClass";

    if (RegisterClassA(&windowClass)) {

        HWND window = CreateWindowExA(
            0, //WS_EX_TOPMOST,
            windowClass.lpszClassName, 
            "Run Around Game", 
            WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
            CW_USEDEFAULT, 
            CW_USEDEFAULT, 
            gameWidth, 
            gameHeight, 
            0, 
            0,
            instance, 
            0
        );
        if (window) {
            programRunning = true;
            HDC deviceContext = GetDC(window);

            initBackBuffer();

            render_command_list renderCommands = {};
            int memoryCapacity = 1 * 1024 * 1024;
            renderCommands.memory.base = malloc(memoryCapacity);
            renderCommands.memory.size = 0;
            renderCommands.memory.capacity = memoryCapacity;

            LARGE_INTEGER lastCounter;
            QueryPerformanceCounter(&lastCounter);

            while (programRunning) {
                MSG message;
                while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
                    switch (message.message) {
                    case WM_QUIT:
                        programRunning = false;
                        break;
                    default:
                        TranslateMessage(&message);
                        DispatchMessageA(&message);
                        break;
                    }
                }

                renderCommands.memory.size = 0;
                clearBuffer((unsigned int *)backBuffer);
                updateGame(&renderCommands);

                unsigned int renderCommandOffset = 0;
                while (renderCommandOffset < renderCommands.memory.size) {
                    render_command_header *header = 
                        (render_command_header *)((char *)renderCommands.memory.base + 
                                                 renderCommandOffset);
                    renderCommandOffset += sizeof(render_command_header);
                    switch(header->type) {
                        default:
                            // error
                            break;
                        case RENDER_COMMAND_RECTANGLE: 
                        {
                            render_rectangle_command *rectCommand = 
                                (render_rectangle_command *)((char *)renderCommands.memory.base + 
                                                            renderCommandOffset);
                            drawRectangle(rectCommand);
                            renderCommandOffset += sizeof(render_rectangle_command);
                        } break;
                        case RENDER_COMMAND_HORIZONTAL_LINE: 
                        {
                            render_horizontal_line_command *lineCommand = 
                                (render_horizontal_line_command *)((char *)renderCommands.memory.base + 
                                                                   renderCommandOffset);
                            drawLine(lineCommand);
                            renderCommandOffset += sizeof(render_horizontal_line_command);
                        } break;
                    }
                }

                drawSceneToWindow(window, deviceContext);

                // Sleep for any leftover time
                LARGE_INTEGER workCounter;
                QueryPerformanceCounter(&workCounter);
                long long counterElapsed = workCounter.QuadPart - lastCounter.QuadPart;
                float elapsedMS = (1000.0f * (float)counterElapsed) / (float)perfCountFrequency;
                if (elapsedMS < targetMSPerFrame) {
                    DWORD sleepMS = (DWORD)(targetMSPerFrame - elapsedMS);
                    if (sleepMS > 0) {
                        Sleep(sleepMS);
                    }
                }

                QueryPerformanceCounter(&lastCounter);
            }
        }
    }
}
