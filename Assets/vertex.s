#version 330

attribute vec3 position;

in vec3 colour;

smooth out vec2 UV;

void main() {
	gl_Position = vec4(position, 0);
	UV = vec2(1,1);
}
