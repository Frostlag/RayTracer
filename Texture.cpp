#include "Texture.hpp"
#include "Utilities.hpp"
#include <lodepng/lodepng.h>
#include <iostream>
using namespace std;
using namespace glm;

Texture::Texture():valid(false), width(0), height(0){

}

Texture::Texture(const string filename){
	unsigned error = lodepng::decode(image, width, height, filename);
	if (!error)
		valid = true;
	else{
		cerr << "Unable to open texture " << filename << endl;
		return;
	}
	//cout << "Done creating texture ";
	//cout << width << " " << height << endl;

}

vec3 Texture::getColour(unsigned x, unsigned y){
	if (x >= width || y >= height)
		throw string("Requested pixel outside of texture");
	unsigned base = (y * width + x) * 4;
	// vec3 ret = vec3(image[base], image[base+1], image[base+2]);
	// cout << ret << endl;
	return vec3(image[base], image[base+1], image[base+2]) / 255;

}

vec3 Texture::getColour(float x, float y){
	unsigned ix = x * width, iy = y * height;
	return getColour(ix, iy);

}

vec3 Texture::getColour(double x, double y){
	//cout << x << " " << y << endl;
	//cout << height << endl;
	unsigned ix = x * width, iy = y * height;
	return getColour(ix, iy);

}
