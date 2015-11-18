#version 330
uniform sampler2D renderedTexture;
smooth in vec2 UV;
out vec4 fragColour;

void main() {

	//fragColour = texture(renderedTexture, UV);
	fragColour = texture(renderedTexture,UV);
}
