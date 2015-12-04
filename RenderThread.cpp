#include <vector>
#include "RenderThread.hpp"
#include "BlockList.hpp"
#include "Primitive.hpp"
#include "GeometryNode.hpp"
#include "PhongMaterial.hpp"
#include "Utilities.hpp"
using namespace std;
using namespace glm;


extern bool super;
extern int supersub;
extern PhongMaterial defaultMat;
extern float reflectionThreshold;
extern float myEpsilon;

#define MY_RAND_MAX 256

std::list<RenderThread*> RenderThread::Threads;
std::list<RenderThread*> RenderThread::WorkingThreads;

RenderThread::RenderThread(SceneNode * root, Image * image, const glm::vec3 eye, const glm::vec3 view, const glm::vec3 up, double fovy, glm::vec3 ambient, const std::list<Light *> lights,
    glm::vec4 E, glm::vec3 baseZ, glm::vec3 u, glm::vec3 r, glm::mat4 invV): root(root), image(image), eye(eye), view(view), up(up), fovy(fovy), ambient(ambient), lights(lights),E(E),
    baseZ(baseZ), u(u), r(r), invV(invV){
    RenderThread::Threads.push_back(this);
    RenderThread::WorkingThreads.push_back(this);

    thethread = thread([this]{this->main();});
}
RenderThread::~RenderThread(){

    thethread.join();
}

PrimitiveCollisions RenderThread::traverseScene(SceneNode * root, vec4 E, vec4 P, mat4 M){
	PrimitiveCollisions ret = PrimitiveCollisions();
#ifndef NOBOUNDING
	if (!root->BoundingVolumeCollide(E, P, M)){
        //cout << "Bounding Miss" << endl;
		return ret;

  }
  // if (root->m_name == "Eleanor")
  //   cout << root->m_name << " Bounding Hit" << endl;
#endif
	ret = root->Collide(E, P, M);

	for (SceneNode* children : root->children){
		PrimitiveCollisions temp = traverseScene(children, E, P, M * root->get_transform());
		if (temp.isEmpty()) continue;
		if (ret.isEmpty())
			ret = temp;
		else if (ret.getCollisions().front().d > temp.getCollisions().front().d)
			ret = temp;
	}
	return ret;
}

vec3 RenderThread::calculateColour(PrimitiveCollisions primitiveCollisions, vec4 E, vec4 P, float adds){
	vec3 ret;

	if (primitiveCollisions.isEmpty())
		return ret;
  //cout << primitiveCollisions.node_name << endl;
  CollisionInfo collisionInfo = primitiveCollisions.getCollisions().front();

  PhongMaterial* mat = primitiveCollisions.mat;
	if (mat == NULL)
		mat = &defaultMat;
	vec3 kd = mat->getKD();
	if(collisionInfo.useTexture)
		kd = collisionInfo.kd;

	ret = ambient * kd * mat->getOpacity();

	if (mat->getReflectivity() > 0)
      ret += calculateReflection(primitiveCollisions, E, P, adds);
  if (mat->getOpacity() < 1){
      //float r0 = pow((1 - mat->getIOR()) / (1 + mat->getIOR()), 2);
      //float rshlick = r0 + (1 - r0) * pow(1 - dot(E, collisionInfo.normal, 5));
      ret += calculateRefraction(primitiveCollisions, E, P, adds);
  }
  ret += calculateLighting(primitiveCollisions, E, P);
	return glm::min(ret,1.0f);
}

vec3 RenderThread::calculateLighting(PrimitiveCollisions primitiveCollisions, vec4 E, vec4 P){
    vec3 ret(0);
    CollisionInfo collisionInfo = primitiveCollisions.getCollisions().front();
    auto attenuationConstant = [] (float r, double falloff[]){
		float c1 = 1, c2 = 1, c3 = 1;
		return 1/(falloff[0] + falloff[1] * r + falloff[2] * r * r);
	};
	vec3 kd = primitiveCollisions.mat->getKD();
	if(collisionInfo.useTexture)
    kd = collisionInfo.kd;
    for( Light * light : lights){
		vec4 l = normalize(vec4(light->position,1) - collisionInfo.position);
		vec4 myEye = collisionInfo.position + l;
		PrimitiveCollisions obstruction = traverseScene( root, myEye, l, mat4());
        float lighteffect = 1;
		if (!obstruction.isEmpty() && obstruction.getCollisions().size() > 1){

			if ( length(obstruction.getCollisions().front().d * l) < length(light->position - vec3(collisionInfo.position))){
                //cout <<obstruction.node_name << endl;
                if (obstruction.mat->getOpacity() < 1){
                    lighteffect = obstruction.mat->getOpacity();
                }
				else continue;
			}

		}
		vec4 v = normalize(-P);
		float n_dot_l = glm::max(dot(collisionInfo.normal, l),0.0f);
		vec3 lightColor = light->colour;
		vec3 diffuse = kd * n_dot_l;

		vec3 specular = vec3(0.0);

		if (n_dot_l > 0){
			vec4 h = (normalize(v + l));
			float n_dot_h = glm::max(dot(collisionInfo.normal, h),0.0f);
			specular = primitiveCollisions.mat->getKS() * pow(n_dot_h, primitiveCollisions.mat->getShininess());
		}

		ret += lighteffect * (vec3(lightColor) * vec3(((diffuse + specular))) * attenuationConstant(length(vec4(light->position,0) - collisionInfo.position), light->falloff));
	}
    return ret;
}

vec3 RenderThread::calculateReflection(PrimitiveCollisions primitiveCollisions, vec4 E, vec4 P, float adds){
    vec3 ret(0);
    if (adds < reflectionThreshold)
        return ret;
    double reflectivity = primitiveCollisions.mat->getReflectivity();
    CollisionInfo first = primitiveCollisions.getCollisions().front();
    vec4 normal = first.normal;
    vec4 newP = normalize(P - 2 * dot(P, normal) * normal);
    vec4 newE = first.position + newP;
    PrimitiveCollisions newCollisions = traverseScene(root, newE, newP, mat4());
    if (!newCollisions.isEmpty()){
        vec3 newColour = calculateColour(newCollisions, newE, newP, adds * reflectivity * length(primitiveCollisions.mat->getKS()) / 3);
        ret += reflectivity * newColour * primitiveCollisions.mat->getKS();
    }
    return ret;
}

vec3 RenderThread::calculateRefraction(PrimitiveCollisions primitiveCollisions, vec4 E, vec4 P, float adds){
    vec3 ret(0);
    if (adds < reflectionThreshold)
        return ret;

    CollisionInfo collisionInfo = primitiveCollisions.getCollisions().front();
    PhongMaterial* mat = primitiveCollisions.mat;
    vec3 normal = vec3(collisionInfo.normal);
    float rindex1 = 1;
    float rindex2 = mat->getIOR();
    float snelldex = rindex1 / rindex2;
    float costheta = -dot(-P,vec4(normal,0));
    float sinsqtheta = pow(snelldex,2) * (1- pow(costheta,2));
    vec3 refracted = normalize((snelldex * vec3(P)) + vec3((snelldex * costheta - sqrt(1 - sinsqtheta)) * normal));
    vec4 newE = collisionInfo.position + myEpsilon * vec4(refracted,0);

    //cout << "Entering from " << newE << " along " << P << ", then travelling along " << refracted << " because of normal " << to_string(normal) << endl;

     PrimitiveCollisions newCollisions = traverseScene(root, newE, vec4(refracted,0), mat4());
    if (newCollisions.isEmpty()){
        cout << "Already out" << endl;
        //return ret;
        return ret;
    }
    if(newCollisions.node_id != primitiveCollisions.node_id){
        return (1 - mat->getOpacity()) * calculateColour(newCollisions, newE, vec4(refracted,0), adds);
    }

    collisionInfo = newCollisions.getCollisions().front();
    normal = vec3(-collisionInfo.normal);
    //cout << "Next internal hit is " << collisionInfo.position << ", normal is " << normal << endl;
    rindex1 = rindex2;
    rindex2 = 1;
    snelldex = rindex1 / rindex2;

    costheta = -dot(refracted,normal);
    sinsqtheta = pow(snelldex,2) * (1- pow(costheta,2));

    int intrefs = 0;
    while (sinsqtheta > 1){
        //cout << "Internal reflection" << endl;
        //cout << "Hit the other end " << newCollisions.getCollisions().size()  << endl;
        //cout << "Ray is " << to_string(refracted) << ", normal is " << to_string(normal) << endl;
        refracted = normalize(refracted -  2 * dot(refracted, normal) * normal);
        //cout << "Reflacted ray is " << to_string(refracted) << endl;
        newE = collisionInfo.position + myEpsilon * vec4(refracted, 0);
        //cout << "Attempting to exit from " << to_string(collisionInfo.position) << " along " << to_string(P) << ", then travelling along " << to_string(refracted) << " because of normal " << to_string(normal) << endl;
        newCollisions = traverseScene(root, newE, vec4(refracted, 0), mat4());
        if (newCollisions.isEmpty()){
            cout << "Already out after bouncing around" << endl;
            //return ret;
            return (1 - mat->getOpacity()) * generateBG(newE, vec4(refracted,0));
        }
        collisionInfo = newCollisions.getCollisions()[1];
        normal = vec3(-collisionInfo.normal);
        costheta = -dot(refracted,normal);
        sinsqtheta = pow(snelldex,2) * (1- pow(costheta,2));
        intrefs++;
        if (intrefs >= 10)
            break;
    }

    refracted = (snelldex * refracted) + ((snelldex * costheta - sqrt(1 - sinsqtheta)) * normal);
    newE = collisionInfo.position + myEpsilon * vec4(refracted,0);
    //cout << "Out of volume, now travelling along " << refracted << " from  " << newE << endl;
    //cout << "Entered via " << P << endl;
    newCollisions = traverseScene(root, newE, vec4(refracted,0), mat4());
    if (newCollisions.isEmpty()){
        //cout << "Exit ray didn't hit anything" << endl;
        //return ret;
        return (1 - mat->getOpacity()) * generateBG(newE, vec4(refracted,0));
    }
    //cout << "Exit ray hit " << newCollisions.node_name << endl;
    return (1 - mat->getOpacity()) * calculateColour(newCollisions, newE, vec4(refracted,0), adds);


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
                                int randX = rand() % MY_RAND_MAX - MY_RAND_MAX >> 2;
                                int randY = rand() % MY_RAND_MAX - MY_RAND_MAX >> 2;
                                //vec4 l = invV * vec4(pixel + i * uo4 + j * ro4, 0);
                                //cout << l << endl;
                                vec4 l = invV * vec4(pixel + i * uo4 + uo4 * (float) randX / (MY_RAND_MAX >> 2) + j * ro4 + uo4 * (float) randX / (MY_RAND_MAX >> 2), 0);
                                //cout << l << endl << endl;
                                PrimitiveCollisions primitiveCollisions = traverseScene(root, E, l, mat4());
                                if (!primitiveCollisions.isEmpty()){
                                    colours.push_back(calculateColour(primitiveCollisions, E, l));
                                }else{
                                    colours.push_back(generateBG(E, l));
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
                        PrimitiveCollisions primitiveCollisions = traverseScene(root, E, l, mat4());

                        if (!primitiveCollisions.isEmpty()){
                            colour = calculateColour(primitiveCollisions, E, l);
                        }else{
                            colour = generateBG(E, l);
                        }
                    }
                    //cout << "Drawing to " << px << "," << h-py-1 << endl;
                    (*image)(px,h-py-1,0) = colour.x;
                    (*image)(px,h-py-1,1) = colour.y;
                    (*image)(px,h-py-1,2) = colour.z;
                    lk.lock();
                    changes.push_back(Changed(px,h-py-1));
                    lk.unlock();
                }
                BlockList::instance->doneRow();

            }

        }
    }catch(string msg){
        cout << msg << endl;
    }
    WorkingThreads.remove(this);
    done = true;
}

list<Changed> RenderThread::getChanges(){
    lk.lock();
    list<Changed> ret = changes;
    changes.clear();
    lk.unlock();
    return ret;
}

vec3 RenderThread::generateBG(int x, int y, int width, int height){
	int repeatTimes = 5;
	float distFromCenter = sqrt(pow(x-width/2,2) + pow(y-height/2,2));
	float ret = distFromCenter/glm::min(height,width)*repeatTimes * 2;
	return vec3(0,0,ret-(int)ret);
}

vec3 RenderThread::generateBG(vec4 E, vec4 P){
    vec3 m_pos(0,0,-1000);
    int gridSize = 100;
    float t = (m_pos.z - E.z)/P.z;
    vec4 p = P * t + E;
    int tx1 = p.x / gridSize;
    int ty1 = p.y / gridSize;
    bool xb = tx1 % 2;
    bool yb = ty1 % 2;

    return xb^yb ? vec3(1,0,0) : vec3(0.5,0.5,0.5);
}
