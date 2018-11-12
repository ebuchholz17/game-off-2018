#include <gl\GL.h>

#include "opengl_renderer.h"
#include "glsl_shaders.h"

// TODO(ebuchholz): figure out how to pack this thing up
static GLuint positionBuffer;
//static GLuint colorBuffer;
static GLuint indexBuffer;

static int loadWGLExtensions (HDC deviceContext) {
    wglGetExtensionsStringARB = (wgl_get_extensions_string_arb *)wglGetProcAddress("wglGetExtensionsStringARB");
    // TODO(ebuchholz): Check for error

    char *wglExtensions = wglGetExtensionsStringARB(deviceContext);
    if (wglExtensions == 0) {
        return 1;
    }
    char *index = wglExtensions;
    char *endIndex;
    int numSupported = 0; // we will require all of the specified extensions, and quit if they are not all available
    while (*index) {
            while ((*index) == ' ') { ++index; };
            endIndex = index;
            while (*endIndex && *endIndex != ' ') { ++endIndex; }

            int length = (int)(endIndex - index);
            if (stringsAreEqual(length, index, "WGL_ARB_create_context")) { ++numSupported; }
            else if (stringsAreEqual(length, index, "WGL_ARB_pixel_format")) { ++numSupported; }
            else if (stringsAreEqual(length, index, "WGL_EXT_swap_control")) { ++numSupported; }

            index = endIndex;
    }
    if (numSupported != 3) {
        return 2;
    }
    // Load the functions we need
    wglCreateContextAttribsARB = (wgl_create_context_attribs_arb *)wglGetProcAddress("wglCreateContextAttribsARB");
    wglChoosePixelFormatARB = (wgl_choose_pixel_format_arb *)wglGetProcAddress("wglChoosePixelFormatARB");
    wglSwapIntervalEXT = (wgl_swap_interval_ext *)wglGetProcAddress("wglSwapIntervalEXT");

    return 0;
}

static void loadOpenGLFunctions () {
    glCreateShader = (gl_create_shader *)wglGetProcAddress("glCreateShader");
    glShaderSource = (gl_shader_source *)wglGetProcAddress("glShaderSource");
    glCompileShader = (gl_compile_shader *)wglGetProcAddress("glCompileShader");
    glCreateProgram = (gl_create_program *)wglGetProcAddress("glCreateProgram");
    glAttachShader = (gl_attach_shader *)wglGetProcAddress("glAttachShader");
    glLinkProgram = (gl_link_program *)wglGetProcAddress("glLinkProgram");
    glGetShaderiv = (gl_get_shader_iv *)wglGetProcAddress("glGetShaderiv");
    glGetProgramiv = (gl_get_program_iv *)wglGetProcAddress("glGetProgramiv");
    glGetShaderInfoLog = (gl_get_shader_info_log *)wglGetProcAddress("glGetShaderInfoLog");
    glGetProgramInfoLog = (gl_get_program_info_log *)wglGetProcAddress("glGetProgramInfoLog");
    glDetachShader = (gl_detach_shader *)wglGetProcAddress("glDetachShader");

    glGetAttribLocation = (gl_get_attrib_location *)wglGetProcAddress("glGetAttribLocation");
    glGenVertexArrays = (gl_gen_vertex_arrays *)wglGetProcAddress("glGenVertexArrays");
    glBindVertexArray = (gl_bind_vertex_array *)wglGetProcAddress("glBindVertexArray");
    glGenBuffers = (gl_gen_buffers *)wglGetProcAddress("glGenBuffers");
    glBindBuffer = (gl_bind_buffer *)wglGetProcAddress("glBindBuffer");
    glBufferData = (gl_buffer_data *)wglGetProcAddress("glBufferData");
    glEnableVertexAttribArray = (gl_enable_vertex_attrib_array *)wglGetProcAddress("glEnableVertexAttribArray");
    glDisableVertexAttribArray = (gl_disable_vertex_attrib_array *)wglGetProcAddress("glDisableVertexAttribArray");
    glVertexAttribPointer = (gl_vertex_attrib_pointer *)wglGetProcAddress("glVertexAttribPointer");
    glUseProgram = (gl_use_program *)wglGetProcAddress("glUseProgram");

    glGetUniformLocation = (gl_get_uniform_location *)wglGetProcAddress("glGetUniformLocation");
    glUniform1i = (gl_uniform_1i *)wglGetProcAddress("glUniform1i");
    glUniform1f = (gl_uniform_1f *)wglGetProcAddress("glUniform1f");
    glUniformMatrix4fv = (gl_uniform_matrix_4fv *)wglGetProcAddress("glUniformMatrix4fv");

    glActiveTexture = (gl_active_texture *)wglGetProcAddress("glActiveTexture");
}

static void loadShader(GLuint *shaderHandle, int shaderType, char *shaderSource, 
                       renderer_memory *memory) 
{
    *shaderHandle = glCreateShader(shaderType);
    const char *constShaderSource = (const char *)shaderSource;
    glShaderSource(*shaderHandle, 1, &constShaderSource, NULL);
    glCompileShader(*shaderHandle);
    int result;
    glGetShaderiv(*shaderHandle, GL_COMPILE_STATUS, &result);
    if (result != 1) {
        memory->debugPrintString("Failed to compile shader\n");
        int length;
        int actualLength;
        glGetShaderiv(*shaderHandle, GL_INFO_LOG_LENGTH, &length);

        char *infoLog = (char *)memory->memory; // completely wreck memory and crash the program
        glGetShaderInfoLog(*shaderHandle, length, &actualLength, infoLog);
        memory->debugPrintString(infoLog);
        assert(0); // Force a crash
    }
}

void loadRendererMesh (loaded_mesh_asset *loadedMesh) {
    // QQQ actually store the mesh key+buffers
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, loadedMesh->positions.count * sizeof(float), loadedMesh->positions.values, GL_STATIC_DRAW);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, loadedMesh->indices.count * sizeof(int), loadedMesh->indices.values, GL_STATIC_DRAW);
}

static void createShaderProgram(openGL_renderer *renderer, shader_type type,
                                char *vertexSource, char *fragmentSource, 
                                renderer_memory *memory) 
{
    // shader_type enum must map inside of the shader array
    shader_program *shaderProgram = &renderer->shaders[type];

    loadShader(&shaderProgram->vertexShader, GL_VERTEX_SHADER, vertexSource, memory);
    loadShader(&shaderProgram->fragmentShader, GL_FRAGMENT_SHADER, fragmentSource, memory);

    shaderProgram->program = glCreateProgram();
    glAttachShader(shaderProgram->program, shaderProgram->vertexShader);
    glAttachShader(shaderProgram->program, shaderProgram->fragmentShader);
    glLinkProgram(shaderProgram->program);
    int result;
    glGetProgramiv(shaderProgram->program, GL_LINK_STATUS, &result);
    if (result != 1) {
        memory->debugPrintString("Failed to link shader\n");
        int length;
        int actualLength;
        glGetProgramiv(shaderProgram->program, GL_INFO_LOG_LENGTH, &length);

        char *infoLog = (char *)memory->memory;
        glGetProgramInfoLog(shaderProgram->program, length, &actualLength, infoLog);
        memory->debugPrintString(infoLog);
        assert(0); // Force a crash
    }
    glDetachShader(shaderProgram->program, shaderProgram->vertexShader);
    glDetachShader(shaderProgram->program, shaderProgram->fragmentShader);
}

int initOpenGL (HWND window, renderer_memory *memory) {
    openGL_renderer *renderer = (openGL_renderer *)memory->memory;

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    HDC deviceContext = GetDC(window);
    renderer->deviceContext = deviceContext;
    int pixelFormat = ChoosePixelFormat(deviceContext, &pfd);
    SetPixelFormat(deviceContext, pixelFormat, &pfd);

    HGLRC glRenderContext = wglCreateContext(deviceContext);
    wglMakeCurrent(deviceContext, glRenderContext);

    int error = loadWGLExtensions(deviceContext);
    if (error != 0) {
        return error;
    }
    int attribList[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 0,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB, // TODO: maybe remove debug
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
        0
    };
    HGLRC betterContext = wglCreateContextAttribsARB(deviceContext, 0, attribList);

    assert(betterContext);

    wglMakeCurrent(deviceContext, betterContext);
    wglDeleteContext(glRenderContext);
    renderer->context = betterContext;

    // TODO(ebuchholz): See if we need to destroy the window and make a new one with different settings
    // Use wglChoosePixelFormatARB, etc.

    // OpenGL 3.x core functions
    loadOpenGLFunctions();
    wglSwapIntervalEXT(1);

    createShaderProgram(renderer, SHADER_TYPE_DEFAULT,
                        defaultVertexShaderSource, defaultFragmentShaderSource, memory);

    // TODO(ebuchholz): Load buffers as part of startup asset loading process
    //float positions[6] = {};
    //positions[0] = 0.0f;
    //positions[1] = 0.33f;
    //positions[2] = -0.5f;
    //positions[3] = -0.33f;
    //positions[4] = 0.5f;
    //positions[5] = -0.33f;
    //glGenBuffers(1, &positionBuffer);
    //glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    //float colors[9] = {};
    //colors[0] = 1.0f;
    //colors[1] = 0.0f;
    //colors[2] = 0.0f;
    //colors[3] = 0.0f;
    //colors[4] = 1.0f;
    //colors[5] = 0.0f;
    //colors[6] = 0.0f;
    //colors[7] = 0.0f;
    //colors[8] = 1.0f;
    //glGenBuffers(1, &colorBuffer);
    //glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    //unsigned int indices[3] = {};
    //indices[0] = 0;
    //indices[1] = 1;
    //indices[2] = 2;
    //glGenBuffers(1, &indexBuffer);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    return 0;
}

void renderFrame (renderer_memory *memory, render_command_list *renderCommands) {
    openGL_renderer *renderer = (openGL_renderer *)memory->memory;

    glViewport(0, 0, 1280, 720);
    //glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLuint program = renderer->shaders[SHADER_TYPE_DEFAULT].program;
    glUseProgram(program);

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionLocation = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, FALSE, 0, 0);

    //glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    //GLint colorLocation = glGetAttribLocation(program, "color");
    //glEnableVertexAttribArray(colorLocation);
    //glVertexAttribPointer(colorLocation, 3, GL_FLOAT, FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    glDrawElements(GL_TRIANGLES, 540, GL_UNSIGNED_INT, 0);

    SwapBuffers(renderer->deviceContext);
}
