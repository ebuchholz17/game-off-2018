#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "../game/run_around_game.cpp"

#include "opengl_renderer.cpp"

static bool programRunning = false;

static int gameWidth = 1280;
static int gameHeight = 720;
static float targetMSPerFrame = 1000.0f / 60.0f;

void DEBUGPrintString (char *string) {
    OutputDebugStringA(string);
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

            renderer_memory rendererMemory;
            // TODO(ebuchholz): wonder how much memory the renderer needs
            rendererMemory.memoryCapacity = 10 * 1024 * 1024;
            rendererMemory.memory = malloc(rendererMemory.memoryCapacity);
            rendererMemory.debugPrintString = &DEBUGPrintString;

            initOpenGL(window, &rendererMemory);

            // Load assets
            asset_list assetList = {};
            assetList.numAssetsToLoad = 0;
            assetList.maxAssetsToLoad = 100;
            assetList.assetsToLoad = 
                (asset_to_load *)malloc(assetList.maxAssetsToLoad * sizeof(asset_to_load));

            getGameAssetList(&assetList);

            memory_arena workingAssetMemory = {};
            workingAssetMemory.capacity = 10 * 1024 * 1024; // 10MB limit for working with asset files?
            workingAssetMemory.base = malloc(workingAssetMemory.capacity);

            for (int i = 0; i < assetList.numAssetsToLoad; ++i) {
                asset_to_load *assetToLoad = assetList.assetsToLoad + i;
                workingAssetMemory.size = 0;
                switch (assetToLoad->type){
                    case ASSET_TYPE_OBJ: 
                    {
                        FILE *objFile = fopen(assetToLoad->path, "r");
                        fseek(f, 0, SEEK_END);
                        int fileSize = ftell(f);
                        rewind(f);

                        char *fileData = malloc(size + 1);
                        fread(fileData, size, 1, f);
                        fclose(f);

                        parseGameAsset(fileData, ASSET_TYPE_OBJ, gameMemory, workingAssetMemory);
                    } break;
                }
            }

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
                            //drawRectangle(rectCommand);
                            renderCommandOffset += sizeof(render_rectangle_command);
                        } break;
                        case RENDER_COMMAND_HORIZONTAL_LINE: 
                        {
                            render_horizontal_line_command *lineCommand = 
                                (render_horizontal_line_command *)((char *)renderCommands.memory.base + 
                                                                   renderCommandOffset);
                            //drawLine(lineCommand);
                            renderCommandOffset += sizeof(render_horizontal_line_command);
                        } break;
                    }
                }

                renderFrame(&rendererMemory, &renderCommands);

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
