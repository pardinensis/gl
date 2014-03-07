#version 330

in vec3 wpos;
uniform float time;

void main() {
	vec3 scaled_wpos = 10 * wpos;
	float scaled_time = 2 * time;

	float magic = 0.5 + 0.5*sin(scaled_time - sin(scaled_wpos.x) + sin(scaled_wpos.z));
	float more_magic = 0.5 + 0.5*sin(scaled_time - sin(scaled_wpos.z) + sin(scaled_wpos.x));
	gl_FragColor = vec4(magic, (1-magic)*more_magic, 1-more_magic, 1);
}