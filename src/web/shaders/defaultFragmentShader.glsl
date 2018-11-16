precision highp float;

varying vec2 vTexCoord;
varying float vLighting;

void main() {
    vec4 baseColor = vec4(0.0, clamp(vTexCoord.s, 0.0, 1.0), clamp(vTexCoord.t, 0.0, 1.0), 1.0);
    vec3 lighting = vec3(0.05, 0.05, 0.05) + (vec3(1.0, 1.0, 1.0) * vLighting);
    gl_FragColor = baseColor * vec4(lighting,1.0);
}
