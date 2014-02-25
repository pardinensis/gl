#version 330

in vec3 wpos;
uniform float time;

void main() {
	float rand = mod(1000 * time - 1000 * wpos.x + 1000 * cos(wpos.y), 256) / 256;
	gl_FragColor = vec4(rand, 0, 1, 1);
}