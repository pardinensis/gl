#version 330

out vec3 wpos;

void main() {
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	wpos = gl_Vertex.xyz;
}