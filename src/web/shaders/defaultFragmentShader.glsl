precision mediump float;

varying vec3 fColor;

void main() {
    gl_FragColor = vec4(fColor.r, fColor.g, fColor.b, 1.0);
}
