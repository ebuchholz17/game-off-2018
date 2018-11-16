#ifndef GLSL_SHADERS_H
#define GLSL_SHADERS_H

char *defaultVertexShaderSource = R"shader(
    attribute vec3 position;
    attribute vec2 texCoord;
    attribute vec3 normal;

    uniform mat4 modelMatrix;
    uniform mat4 viewMatrix;
    uniform mat4 projMatrix;

    varying vec2 vTexCoord;
    varying float vLighting;

    void main() {
        gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
        vTexCoord = texCoord;
        vec4 newNormal = modelMatrix * vec4(normal, 0.0);
        vec4 lightDir = normalize(vec4(1.0, 1.0, 1.0, 0.0));
        float lighting = dot(newNormal, lightDir);
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
        vec4 baseColor = vec4(0.0, clamp(vTexCoord.s, 0.0, 1.0), clamp(vTexCoord.t, 0.0, 1.0), 1.0);
        vec4 lighting = vec4(0.05, 0.05, 0.05, 1.0) + (vec4(1.0, 1.0, 1.0, 1.0) * vLighting);
        gl_FragColor = baseColor * lighting;
    }
)shader";

#endif
