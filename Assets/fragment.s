#version 330
uniform vec2 texCords;
uniform sampler2D renderedTexture;
uniform bool useTexture;

smooth in vec2 UV;
out vec4 fragColour;

void main() {

	if (true){
		fragColour = texture(renderedTexture,UV);
	}else{
		fragColour = vec4(UV.x,UV.y,0,1);
	}

}
