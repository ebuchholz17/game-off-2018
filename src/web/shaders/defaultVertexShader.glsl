attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

varying vec2 vTexCoord;
varying float vLighting;

void main() {
    gl_Position = projMatrix * viewMatrix * vec4(position, 1.0);
    vTexCoord = texCoord;

    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float lighting = dot(normal, lightDir);
    vLighting = lighting;
}
