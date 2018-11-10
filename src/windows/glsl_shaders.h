#ifndef GLSL_SHADERS_H
#define GLSL_SHADERS_H

char *defaultVertexShaderSource = R"shader(
    attribute vec2 position;
    attribute vec3 color;
    
    varying vec3 fColor;
    
    void main() {
        gl_Position = vec4(position, 0.0, 1.0);
        fColor = color;
    }
)shader";

char *defaultFragmentShaderSource = R"shader(
    varying vec3 fColor;
    
    void main() {
        gl_FragColor = vec4(fColor.r, fColor.g, fColor.b, 1.0);
    }
)shader";

#endif
