#include <vector>
#include "RenderThread.hpp"
#include "BlockList.hpp"
#include "Primitive.hpp"
#include "GeometryNode.hpp"
#include "PhongMaterial.hpp"
using namespace std;
using namespace glm;


extern bool super;
extern int supersub;
extern PhongMaterial defaultMat;

std::list<RenderThread*> RenderThread::Threads;

RenderThread::RenderThread(SceneNode * root, Image * image, const glm::vec3 eye, const glm::vec3 view, const glm::vec3 up, double fovy, glm::vec3 ambient, const std::list<Light *> lights,
    glm::vec4 E, glm::vec3 baseZ, glm::vec3 u, glm::vec3 r, glm::mat4 invV): root(root), image(image), eye(eye), view(view), up(up), fovy(fovy), ambient(ambient), lights(lights),E(E),
    baseZ(baseZ), u(u), r(r), invV(invV){
    RenderThread::Threads.push_back(this);

    thethread = thread([this]{this->main();});
}
RenderThread::~RenderThread(){

    thethread.join();
    Threads.remove(this);
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


void RenderThread::main(){
    try{
        vec3 uo4 = u / supersub, ro4 = r / supersub;
        int h = image->height();
        int w = image->width();
        while(true){
            Block block = BlockList::instance->getBlock();
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
                }
                BlockList::instance->doneRow();

            }

        }
    }catch(string msg){
        cout << msg << endl;
    }
    done = true;
}

vec3 RenderThread::generateBG(int x, int y, int width, int height){
	int repeatTimes = 5;
	float distFromCenter = sqrt(pow(x-width/2,2) + pow(y-height/2,2));
	float ret = distFromCenter/glm::min(height,width)*repeatTimes * 2;
	return vec3(0,0,ret-(int)ret);
}
