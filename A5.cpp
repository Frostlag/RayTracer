#include <glm/ext.hpp>
#include <thread>
#include <vector>
#include "A5.hpp"
#include "GeometryNode.hpp"
#include "BlockList.hpp"
using namespace std;
using namespace glm;

extern int subdivisions;
extern int threads;
bool super = false;
int supersub = 4;
volatile unsigned int squareRowsDone = 0;
BlockList *blockList;
PhongMaterial defaultMat(vec3(0.2,0.2,0.2),vec3(0.1,0.1,0.1),40);


vec3 generateBG(int x, int y, int width, int height){
	//return vec3(0);
	int repeatTimes = 5;
	float distFromCenter = sqrt(pow(x-width/2,2) + pow(y-height/2,2));
	float ret = distFromCenter/glm::min(height,width)*repeatTimes * 2;
	return vec3(0,0,ret-(int)ret);
}

void A4_Render_Thread(
	// What to render
	SceneNode * root,
	Image * image,
	const glm::vec3 eye,
	const glm::vec3 view,
	const glm::vec3 up,
	double fovy,
	// Lighting parameters
	const glm::vec3 ambient,
	const std::list<Light *> lights,
	vec4 E, vec3 baseZ, vec3 u, vec3 r, mat4 invV,
	BlockList* blockList
){
	try{
		vec3 uo4 = u / supersub, ro4 = r / supersub;
		int h = image->height();
		int w = image->width();
		while(true){
			Block block = blockList->getBlock();
			int startX = block.startX, startY = block.startY, endX = block.endX, endY = block.endY, startPX = block.startPX, startPY = block.startPY;
			for (int y = startY, py = startPY ; y < endY; ++y, ++py) {

				for (int x = startX, px = startPX; x < endX; ++x, ++px) {
					vec3 colour;
					vec3 pixel = normalize(baseZ + u * y + r * x);
					if (super){
						if (supersub % 2 == 0){
							pixel = pixel + (uo4 + ro4) * 0.5;
						}
						vector<vec3> colours;
						for (int i = -supersub / 2; i < supersub / 2; i++ ){
							for (int j = -supersub / 2; j < supersub / 2; j++){
								vec4 l = invV * vec4(pixel + i * uo4 + j * ro4, 0);
								CollisionInfo collisionInfo = traverseScene(root, E, l, mat4());
								if (collisionInfo.isValid){
									colours.push_back(calculateColour(root, lights, ambient, collisionInfo, E, l));
								}else{
									colours.push_back(generateBG(px, h-py-1, w, h));
								}
							}
						}
						vec3 sumcolours(0.0f);
						for ( int i = 0; i < colours.size(); i++){

							sumcolours += colours[i];
						}

						colour = sumcolours / ((float) colours.size());
						//cout << to_string(colour) << endl;
					}
					else{
						vec4 l = invV * vec4(pixel, 0);
						// cout << "pixel: " << to_string(pixel) << endl;
						// cout << "l: "<< to_string(l) << endl;
						CollisionInfo collisionInfo = traverseScene(root, E, l, mat4());

						if (collisionInfo.isValid){
							colour = calculateColour(root, lights, ambient, collisionInfo, E, l);
						}else{
							colour = generateBG(px, h-py-1, w, h);
						}
					}
					//cout << "Drawing to " << px << "," << h-py-1 << endl;
					(*image)(px,h-py-1,0) = colour.x;
					(*image)(px,h-py-1,1) = colour.y;
					(*image)(px,h-py-1,2) = colour.z;

					// Red: increasing from top to bottom
					//image(px, py, 0) = (double)(y+h/2) / h;
					// Green: increasing from left to right
					//image(px, py, 1) = (double)(x+w/2) / w;
					// Blue: in lower-left and upper-right corners
					//image(px, py, 2) = (((y+h/2) < h/2 && (x+w/2) < w/2)
					//		|| ((y+h/2) >= h/2 && (x+w/2) >= w/2)) ? 1.0 : 0.0;
					//cout << px << endl;
				}
				blockList->doneRow();

			}

		}
	}catch(string msg){
		cout << msg << endl;
	}






}

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
) {
	// Fill in raytracing code here...

	std::cout << "Calling A4_Render(\n" <<
		"\t" << *root <<
		"\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
		"\t" << "eye:  " << glm::to_string(eye) << std::endl <<
		"\t" << "view: " << glm::to_string(view) << std::endl <<
		"\t" << "up:   " << glm::to_string(up) << std::endl <<
		"\t" << "fovy: " << fovy << std::endl <<
		"\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
		"\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;

	double aspect;
	aspect = (float)image.width() / (float)image.height();
	float n = (image.height() / 2) / tan(radians((float)fovy/2));
	float gridHeight = image.height() / n;
	float gridWidth = image.width() / n;
	std::cout << "aspect: " << aspect << std::endl;
	std::cout << "n: " << n << std::endl;
	std::cout << "H,W: " << gridHeight << " " << gridWidth << std::endl;
	std::cout << "h,w: " << image.height() << " " << image.width() << std::endl;
	vec3 u = (normalize(up * gridHeight)) / image.height();
	vec3 r = (normalize(cross(up,vec3(0,0,n)) * gridWidth) / image.width());
	cout << "u: " << to_string(u) << endl;
	cout << "r: " << to_string(r) << endl;

	int h = image.height();
	int w = image.width();

	mat4 V = lookAt(eye, view, up);
	mat4 invV = inverse(V);
	//cout << to_string(V) << endl;
	//cout << to_string(invV) << endl;

	vec3 baseZ = vec3(0,0,-1);
	if (h % 2 == 0){
		baseZ = baseZ + u*0.5;
	}
	if (w % 2 == 0){
		baseZ = baseZ + r*0.5;
	}

	vec4 E = vec4((float)eye.x,eye.y,eye.z,1);

	//cout << -h/2 << " " << h/2 << endl;

	blockList = new BlockList(h, w, subdivisions);
	root->reboundVolume();

	vector<thread*> threadvector;
	for (int i = 0; i < threads; i++){
		thread* temp = new thread(A4_Render_Thread, root, &image, eye, view, up, fovy, ambient, lights, E, baseZ, u, r, invV, blockList);
		threadvector.push_back(temp);
	}

	cout << "Threads: " << threadvector.size() << endl;


	// Single Threaded
	// float lastPer = 0;
	// for (int y = -h/2, py = 0 ; y < hcap; ++y, ++py) {
	//
	// 	for (int x = -w/2, px = 0; x < wcap; ++x, ++px) {
	// 		vec3 pixel =normalize(baseZ + u * y + r * x);
	// 		vec4 l = invV * vec4(pixel, 0);
	// 		// cout << "pixel: " << to_string(pixel) << endl;
	// 		// cout << "l: "<< to_string(l) << endl;
	// 		CollisionInfo collisionInfo = traverseScene(root, E, l, mat4());
	// 		vec3 colour;
	// 		if (collisionInfo.isValid){
	// 			colour = calculateColour(root, lights, ambient, collisionInfo, E, l);
	// 		}else{
	// 			colour = generateBG(px, h-py-1, w, h);
	// 		}
	// 		image(px,h-py-1,0) = colour.x;
	// 		image(px,h-py-1,1) = colour.y;
	// 		image(px,h-py-1,2) = colour.z;
	//
	// 		// Red: increasing from top to bottom
	// 		//image(px, py, 0) = (double)(y+h/2) / h;
	// 		// Green: increasing from left to right
	// 		//image(px, py, 1) = (double)(x+w/2) / w;
	// 		// Blue: in lower-left and upper-right corners
	// 		//image(px, py, 2) = (((y+h/2) < h/2 && (x+w/2) < w/2)
	// 		//		|| ((y+h/2) >= h/2 && (x+w/2) >= w/2)) ? 1.0 : 0.0;
	// 	}
	// 	if ( (float)py / h * 100 - lastPer > 1){
	// 		cout << (float)py / h * 100 << "% done" << endl;
	// 		lastPer = (float)py / h * 100;
	// 	}
	//
	//
	// }
	for(int i = 0; i < threadvector.size(); i++){
		threadvector[i]->join();

		delete threadvector[i];
	}
	delete blockList;
}




CollisionInfo traverseScene(SceneNode * root, vec4 E, vec4 P, mat4 M){
	CollisionInfo ret = CollisionInfo();
#ifndef NOBOUNDING
	if (!root->BoundingVolumeCollide(E, P, M).isValid)
		return ret;
#endif
	if (root->m_nodeType == NodeType::GeometryNode){
		GeometryNode* geometryNode = static_cast<GeometryNode*>(root);
		ret = geometryNode->Collide(E, P, M);


	}
	for (SceneNode* children : root->children){
		CollisionInfo temp = traverseScene(children, E, P, M * root->get_transform());
		if (!temp.isValid) continue;
		if (!ret.isValid)
			ret = temp;
		else if (ret.d > temp.d)
			ret = temp;
	}
	return ret;
}

vec3 calculateColour(SceneNode * root, const list<Light *> & lights, const vec3 & ambient, CollisionInfo collisionInfo, vec4 E, vec4 P){
	vec3 ret;

	if (!collisionInfo.isValid)
		return ret;

	if (collisionInfo.mat == NULL)
		collisionInfo.mat = &defaultMat;


	auto attenuationConstant = [] (float r, double falloff[]){
		float c1 = 1, c2 = 1, c3 = 1;
		return 1/(falloff[0] + falloff[1] * r + falloff[2] * r * r);
	};

	ret = ambient * collisionInfo.mat->getKD();

	for( Light * light : lights){
		vec4 l = normalize(vec4(light->position,1) - collisionInfo.position);
		vec4 myEye = collisionInfo.position + l * 1e-2;
		CollisionInfo obstruction = traverseScene( root, myEye, l, mat4());
		if (obstruction.isValid){
			if ( length(obstruction.d * l) < length(light->position - vec3(collisionInfo.position))){
				continue;
			}

		}
		//cout << collisionInfo.node_name << " not obstructed" << endl;
		vec4 v = normalize(-P);
		float n_dot_l = glm::max(dot(collisionInfo.normal, l),0.0f);
		vec3 lightColor = light->colour;
		vec3 kd = collisionInfo.mat->getKD();
		vec3 diffuse = kd * n_dot_l;

		vec3 specular = vec3(0.0);

		if (n_dot_l > 0){
			vec4 h = (normalize(v + l));
			float n_dot_h = glm::max(dot(collisionInfo.normal, h),0.0f);
			specular = collisionInfo.mat->getKS() * pow(n_dot_h, collisionInfo.mat->getShininess());
		}

		ret += vec3(lightColor) * vec3(((diffuse + specular))) * attenuationConstant(length(vec4(light->position,0) - collisionInfo.position), light->falloff);
	}
		//cout << to_string((diffuse + specular)) << endl;
	// if (collisionInfo.node_name == "plane"){
	// 	cout << "Plane normal is " << to_string(collisionInfo.normal) << endl;
	// }
	return glm::min(ret,1.0f);
}
