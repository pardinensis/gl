#version 330

in vec3 wpos;
uniform float time;

void main() {
	float rand = 0.5+0.5*sin((1000 * time - 1000 * wpos.x + 1000 * cos(wpos.y)) / 100);
	gl_FragColor = vec4(0, 1-rand, rand, 1);
}