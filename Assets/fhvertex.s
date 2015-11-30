#version 330


in vec2 position;

in vec3 colour;

smooth out vec2 UV;

void main() {
	gl_Position = vec4(position, 0, 1);

}
