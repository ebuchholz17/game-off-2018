#ifndef GLSL_SHADERS_H
#define GLSL_SHADERS_H

char *defaultVertexShaderSource = R"shader(
    attribute vec3 position;
    
    void main() {
        gl_Position = vec4(position, 1.0);
    }
)shader";

char *defaultFragmentShaderSource = R"shader(
    void main() {
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
)shader";

#endif
