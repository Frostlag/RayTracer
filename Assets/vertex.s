#version 330


in vec2 position;

in vec3 colour;

smooth out vec2 UV;

void main() {
	gl_Position = vec4(position, 1.0, 1);
	vec2 tempPos = position;
	tempPos.y *= -1;
	UV = (vec2(1) + tempPos) * 0.5;

}
