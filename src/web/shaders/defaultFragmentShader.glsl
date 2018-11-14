precision highp float;

varying vec2 vTexCoord;
varying float vLighting;

void main() {
    gl_FragColor = vec4(0.0, vTexCoord.s * vLighting, vTexCoord.t * vLighting, 1.0);
}
