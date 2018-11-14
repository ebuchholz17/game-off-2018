#ifndef GLSL_SHADERS_H
#define GLSL_SHADERS_H

char *defaultVertexShaderSource = R"shader(
    attribute vec3 position;
    attribute vec2 texCoord;
    attribute vec3 normal;

    varying vec2 vTexCoord;
    varying float vLighting;

    void main() {
        gl_Position = vec4(position, 1.0);
        vTexCoord = texCoord;
        vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
        float lighting = dot(normal, lightDir);
        vLighting = lighting;
    }
)shader";

//char *defaultVertexShaderSource = R"shader(
//    attribute vec3 position;
//
//    uniform mat4 modelMatrix;
//    uniform mat4 viewProjMatrix;
//    
//    void main() {
//        gl_Position = vec4(position, 1.0);
//    }
//)shader";

char *defaultFragmentShaderSource = R"shader(
    varying vec2 vTexCoord;
    varying float vLighting;

    void main() {
        gl_FragColor = vec4(0.0, vTexCoord.s * vLighting, vTexCoord.t * vLighting, 1.0);
    }
)shader";

#endif
