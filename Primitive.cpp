
#include <iostream>
#include <glm/ext.hpp>
#include <glm/gtx/norm.hpp>
#include <vector>
#include "Primitive.hpp"
#include "polyroots.hpp"
#include "Utilities.hpp"

using namespace std;
using namespace glm;

Primitive::Primitive():texture(NULL){

}

Primitive::~Primitive()
{
}

void Primitive::setTexture(Texture *texture){
    if(texture == NULL || !texture->isValid())
        return;
    this->texture = texture;
}

void Sphere::draw(mat4 M, unsigned int nodeId){


}

PrimitiveCollisions Sphere::Collide(  vec4 E, vec4 P, glm::mat4 M ){
    PrimitiveCollisions ret;
    vec4 center = vec4(0,0,0,1);
    mat4 invM = inverse(M);
    vec4 invE = invM * E;
    vec4 invP = invM * P;
    vec4 diff = invE - center;
    float invPsq = pow(length(invP),2);

    double A = length2(invP);
	double B = 2 * (invP.x * diff.x + invP.y * diff.y + invP.z * diff.z);
	double C = length2(diff) - 1;
	double roots[2];

	size_t result = quadraticRoots(A, B, C, roots);

	if (result < 1)
		return ret;

    for (int i = 0; i < result; i++){
        CollisionInfo temp = CollisionInfo(roots[i], E + roots[i] * P,
                normalize(vec4(transpose(inverse(mat3(M))) * vec3(invE + roots[i] * invP), 0)));
        if (texture != NULL && texture->isValid()){
            temp.useTexture = true;
            vec4 normal = normalize(vec4(invE + roots[i] * invP));
            temp.kd = texture->getColour(0.5 + atan(normal.z, normal.x) / (pi<float>() * 2), 0.5 - asin(normal.y) / pi<float>());
        }
        ret.addCollision(temp);
    }

	return ret;
}

pair<glm::vec4,glm::vec4> Sphere::getBounds(){
	return {vec4(-1,-1,-1,1),vec4(1,1,1,1)};
}


Sphere::~Sphere()
{
}

PrimitiveCollisions Cube::Collide(  vec4 E, vec4 P, glm::mat4 M ){
    PrimitiveCollisions ret;
    mat4 invM = inverse(M);
    vec4 invE = invM * E;
    vec4 invP = invM * P;

    vec3 m_pos = vec3(0);
    float m_size = 1;
    vec3 m_pos2 = m_pos + vec3(m_size,m_size,m_size);

    float tx1 = (m_pos.x - invE.x)/invP.x;
    float tx2 = (m_pos2.x - invE.x)/invP.x;

    float tmin = glm::min(tx1,tx2);
    float tmax = glm::max(tx1,tx2);

    float ty1 = (m_pos.y - invE.y)/invP.y;
    float ty2 = (m_pos2.y - invE.y)/invP.y;

    tmin = glm::max(tmin,glm::min(ty1,ty2));
    tmax = glm::min(tmax,glm::max(ty1,ty2));

    float tz1 = (m_pos.z - invE.z)/invP.z;
    float tz2 = (m_pos2.z - invE.z)/invP.z;

    tmin = glm::max(tmin,glm::min(tz1,tz2));
    tmax = glm::min(tmax,glm::max(tz1,tz2));

    if (tmin > tmax && tmin > 0 && !isnan(tmin)){

        return ret;
    }

    vector<pair<vec3,vec3>> planeInfo = {
        {m_pos,vec3(0,0,-1)},
        {m_pos,vec3(0,-1,0)},
        {m_pos,vec3(-1,0,0)},
        {m_pos2,vec3(0,0,1)},
        {m_pos2,vec3(0,1,0)},
        {m_pos2,vec3(1,0,0)},
    };

    vector<pair<vector<vec3>,vector<vec3>>> squareInfo = {
        {{m_pos + vec3(0,0,0), m_pos + vec3(m_size,0,0), m_pos + vec3(m_size,m_size,0), m_pos + vec3(0,m_size,0)},
            {vec3(0,1,0),vec3(-1,0,0),vec3(0,-1,0),vec3(1,0,0)}},
        {{m_pos + vec3(0,0,0), m_pos + vec3(m_size,0,0), m_pos + vec3(m_size,0,m_size), m_pos + vec3(0,0,m_size)},
            {vec3(0,0,1),vec3(-1,0,0),vec3(0,0,-1),vec3(1,0,0)}},
        {{m_pos + vec3(0,0,0), m_pos + vec3(0,m_size,0), m_pos + vec3(0,m_size,m_size), m_pos + vec3(0,0,m_size)},
            {-vec3(0,0,-1),-vec3(0,1,0),-vec3(0,0,1),-vec3(0,-1,0)}},
        {{m_pos2 - vec3(0,0,0), m_pos2 - vec3(m_size,0,0), m_pos2 - vec3(m_size,m_size,0), m_pos2 - vec3(0,m_size,0)},
            {-vec3(0,1,0),-vec3(-1,0,0),-vec3(0,-1,0),-vec3(1,0,0)}},
        {{m_pos2 - vec3(0,0,0), m_pos2 - vec3(m_size,0,0), m_pos2 - vec3(m_size,0,m_size), m_pos2 - vec3(0,0,m_size)},
            {-vec3(0,0,1),-vec3(-1,0,0),-vec3(0,0,-1),-vec3(1,0,0)}},
        {{m_pos2 - vec3(0,0,0), m_pos2 - vec3(0,m_size,0), m_pos2 - vec3(0,m_size,m_size), m_pos2 - vec3(0,0,m_size)},
            {vec3(0,0,-1),vec3(0,1,0),vec3(0,0,1),vec3(0,-1,0)}}
    };
    vec4 center = vec4(0.5);
    for( int i  = 0; i < 6; i++){
        float d = dot(vec4(planeInfo[i].first,1) -  invE, vec4(planeInfo[i].second,0)) / dot(invP, vec4(planeInfo[i].second,0));
        if (d <= 0) continue;
        vec4 potentialPoint = invE + d * invP;
        bool isIn = true;
        for ( int j = 0; j < 4; j++){
            float t = dot(vec3(potentialPoint) - squareInfo[i].first[j], squareInfo[i].second[j]);
            if (t < 0) {
                isIn = false;
                break;
            }
        }
        if (!isIn)
            continue;
        CollisionInfo temp = CollisionInfo(d, M * potentialPoint, normalize(vec4(transpose(inverse(mat3(M))) * planeInfo[i].second, 0)));
        if (texture != NULL && texture->isValid()){
            temp.useTexture = true;
            vec4 normal = normalize(potentialPoint - center);
            temp.kd = texture->getColour(0.5 + atan(normal.z, normal.x) / (pi<float>() * 2), 0.5 - asin(normal.y) / pi<float>());
        }
        ret.addCollision(temp);

    }

    return ret;
}

pair<glm::vec4,glm::vec4> Cube::getBounds(){
	return {vec4(0,0,0,1), vec4(1,1,1,1)};
}

void Cube::draw(mat4 M, unsigned int nodeId){


}

Cube::~Cube()
{
}

PrimitiveCollisions NonhierSphere::Collide(  vec4 E, vec4 P, glm::mat4 M ){
    PrimitiveCollisions ret;
    vec4 center = vec4(m_pos,1);
    mat4 invM = inverse(M);
    vec4 invE = invM * E;
    vec4 invP = invM * P;
    vec4 diff = invE - center;
    float invPsq = pow(length(invP),2);

	double A = length2(invP);
	double B = 2 * (invP.x * diff.x + invP.y * diff.y + invP.z * diff.z);
	double C = length2(diff) - pow(m_radius,2);
	double roots[2];

	size_t result = quadraticRoots(A, B, C, roots);

	if (result < 1)
		return ret;

    for (int i = 0; i < result; i++){
		CollisionInfo temp = CollisionInfo(roots[i], E + roots[i] * P,
				normalize(vec4(transpose(inverse(mat3(M))) * (vec3(invE + roots[i] * invP) - m_pos), 0)));
        if (texture != NULL && texture->isValid()){
    		temp.useTexture = true;
    		vec4 normal = normalize(vec4(invE + roots[i] * invP) - vec4(m_pos,1));
    		temp.kd = texture->getColour(0.5 + atan(normal.z, normal.x) / (pi<float>() * 2), 0.5 - asin(normal.y) / pi<float>());
        }

        ret.addCollision(temp);
    }

    return ret;

}

pair<glm::vec4,glm::vec4> NonhierSphere::getBounds(){
	return {vec4(m_pos - vec3(m_radius),1),vec4(m_pos + vec3(m_radius),1)};
}

void NonhierSphere::draw(mat4 M, unsigned int nodeId){


}

NonhierSphere::~NonhierSphere()
{
}

PrimitiveCollisions NonhierBox::Collide(vec4 E, vec4 P, glm::mat4 M){
    PrimitiveCollisions ret;
    mat4 invM = inverse(M);
    vec4 invE = invM * E;
    vec4 invP = invM * P;

    vec3 m_pos2 = m_pos + vec3(m_size,m_size,m_size);

    float tx1 = (m_pos.x - invE.x)/invP.x;
    float tx2 = (m_pos2.x - invE.x)/invP.x;

    float tmin = glm::min(tx1,tx2);
    float tmax = glm::max(tx1,tx2);

    float ty1 = (m_pos.y - invE.y)/invP.y;
    float ty2 = (m_pos2.y - invE.y)/invP.y;

    tmin = glm::max(tmin,glm::min(ty1,ty2));
    tmax = glm::min(tmax,glm::max(ty1,ty2));

    float tz1 = (m_pos.z - invE.z)/invP.z;
    float tz2 = (m_pos2.z - invE.z)/invP.z;

    tmin = glm::max(tmin,glm::min(tz1,tz2));
    tmax = glm::min(tmax,glm::max(tz1,tz2));

    if (tmin > tmax && tmin > 0 && !isnan(tmin)){

        return ret;
    }

    vector<pair<vec3,vec3>> planeInfo = {
        {m_pos,vec3(0,0,-1)},
        {m_pos,vec3(0,-1,0)},
        {m_pos,vec3(-1,0,0)},
        {m_pos2,vec3(0,0,1)},
        {m_pos2,vec3(0,1,0)},
        {m_pos2,vec3(1,0,0)},
    };

    vector<pair<vector<vec3>,vector<vec3>>> squareInfo = {
        {{m_pos + vec3(0,0,0), m_pos + vec3(m_size,0,0), m_pos + vec3(m_size,m_size,0), m_pos + vec3(0,m_size,0)},
            {vec3(0,1,0),vec3(-1,0,0),vec3(0,-1,0),vec3(1,0,0)}},
        {{m_pos + vec3(0,0,0), m_pos + vec3(m_size,0,0), m_pos + vec3(m_size,0,m_size), m_pos + vec3(0,0,m_size)},
            {vec3(0,0,1),vec3(-1,0,0),vec3(0,0,-1),vec3(1,0,0)}},
        {{m_pos + vec3(0,0,0), m_pos + vec3(0,m_size,0), m_pos + vec3(0,m_size,m_size), m_pos + vec3(0,0,m_size)},
            {-vec3(0,0,-1),-vec3(0,1,0),-vec3(0,0,1),-vec3(0,-1,0)}},
        {{m_pos2 - vec3(0,0,0), m_pos2 - vec3(m_size,0,0), m_pos2 - vec3(m_size,m_size,0), m_pos2 - vec3(0,m_size,0)},
            {-vec3(0,1,0),-vec3(-1,0,0),-vec3(0,-1,0),-vec3(1,0,0)}},
        {{m_pos2 - vec3(0,0,0), m_pos2 - vec3(m_size,0,0), m_pos2 - vec3(m_size,0,m_size), m_pos2 - vec3(0,0,m_size)},
            {-vec3(0,0,1),-vec3(-1,0,0),-vec3(0,0,-1),-vec3(1,0,0)}},
        {{m_pos2 - vec3(0,0,0), m_pos2 - vec3(0,m_size,0), m_pos2 - vec3(0,m_size,m_size), m_pos2 - vec3(0,0,m_size)},
            {vec3(0,0,-1),vec3(0,1,0),vec3(0,0,1),vec3(0,-1,0)}}
    };

    vec4 center = vec4(m_pos + vec3(m_size/2),1);

    for( int i  = 0; i < 6; i++){
        float d = dot(vec4(planeInfo[i].first,1) -  invE, vec4(planeInfo[i].second,0)) / dot(invP, vec4(planeInfo[i].second,0));
        if (d <= 0) continue;
        vec4 potentialPoint = invE + d * invP;
        bool isIn = true;
        for ( int j = 0; j < 4; j++){
            float t = dot(vec3(potentialPoint) - squareInfo[i].first[j], squareInfo[i].second[j]);
            if (t < 0) {
                isIn = false;
                break;
            }

        }
        if (!isIn)
            continue;

        CollisionInfo temp = CollisionInfo(d, M * potentialPoint, normalize(vec4(transpose(inverse(mat3(M))) * planeInfo[i].second, 0)));
        if (texture != NULL && texture->isValid()){
            temp.useTexture = true;
            vec4 normal = normalize(potentialPoint - center);
            temp.kd = texture->getColour(0.5 + atan(normal.z, normal.x) / (pi<float>() * 2), 0.5 - asin(normal.y) / pi<float>());
        }
        ret.addCollision(temp);
    }

    return ret;
}

bool NonhierBox::Bounds(vec4 E, vec4 P, glm::mat4 M){
    PrimitiveCollisions ret;
    mat4 invM = inverse(M);
    vec4 invE = invM * E;
    vec4 invP = invM * P;
    // cout << to_string(invM) << endl;
    // cout << to_string(E) << " " << to_string(invE) << endl;
    vec3 m_pos2 = m_pos + vec3(m_size,m_size,m_size);

    float tx1 = (m_pos.x - invE.x)/invP.x;
    float tx2 = (m_pos2.x - invE.x)/invP.x;

    float tmin = glm::min(tx1,tx2);
    float tmax = glm::max(tx1,tx2);

    float ty1 = (m_pos.y - invE.y)/invP.y;
    float ty2 = (m_pos2.y - invE.y)/invP.y;

    tmin = glm::max(tmin,glm::min(ty1,ty2));
    tmax = glm::min(tmax,glm::max(ty1,ty2));

    float tz1 = (m_pos.z - invE.z)/invP.z;
    float tz2 = (m_pos2.z - invE.z)/invP.z;

    tmin = glm::max(tmin,glm::min(tz1,tz2));
    tmax = glm::min(tmax,glm::max(tz1,tz2));
    //cout << to_string(E) << " " << tmax << " " << tmin << endl;
    return tmax >= tmin || tmin < 0;

}


pair<glm::vec4,glm::vec4> NonhierBox::getBounds(){
	return {vec4(m_pos,1),vec4(m_pos +  vec3(m_size),1)};
}

void NonhierBox::draw(mat4 M, unsigned int nodeId){


}

NonhierBox::~NonhierBox()
{
}


PrimitiveCollisions Cone::Collide(glm::vec4 E,glm::vec4 P, glm::mat4 M){
    PrimitiveCollisions ret;
    mat4 invM = inverse(M);
    vec4 invE = invM * E;
    vec4 invP = invM * P;


	double A = pow(invP.x,2) + pow(invP.z,2) - pow(invP.y,2);
	double B = 2 * (invE.x * invP.x + invE.z * invP.z - invE.y * invP.y);
	double C = pow(invE.x,2) + pow(invE.z,2) - pow(invE.y,2);
	double roots[2];

	size_t result = quadraticRoots(A, B, C, roots);
    //cout << result << endl;
    if (result < 1)
        return ret;
    vec4 center = vec4(0,0.5,0,0);

    for (int i = 0; i < result; i++){
        vec4 ourPoint = (invE + roots[i] * invP);

        if (ourPoint.y >= 0 && ourPoint.y <= 1 ){
            vec3 temp2 = normalize(vec3(ourPoint.x, 0, ourPoint.z));
            temp2.y = -1;
            CollisionInfo temp = CollisionInfo(roots[i], E + roots[i] * P, normalize(vec4(transpose(inverse(mat3(M))) * temp2, 0)));
            if (texture != NULL && texture->isValid()){
                temp.useTexture = true;
                vec4 normal = normalize(ourPoint - center);
                temp.kd = texture->getColour(0.5 + atan(normal.z, normal.x) / (pi<float>() * 2), 0.5 - asin(normal.y) / pi<float>());
            }
            ret.addCollision(temp);
        }
    }

    vec3 m_pos = vec3(0,1,0);
    float ty1 = (m_pos.y - invE.y)/invP.y;
    vec4 ourPoint = (invE + ty1 * invP);
    if (pow(ourPoint.x, 2) + pow(ourPoint.z, 2) <= 1){
        CollisionInfo temp = CollisionInfo(ty1, E + ty1 * P, normalize(vec4(transpose(inverse(mat3(M))) * vec3(0, 1, 0), 0)));
        if (texture != NULL && texture->isValid()){
            temp.useTexture = true;
            vec4 normal = normalize(invE + ty1 * invP - center);
            temp.kd = texture->getColour(0.5 + atan(normal.z, normal.x) / (pi<float>() * 2), 0.5 - asin(normal.y) / pi<float>());
        }
        ret.addCollision(temp);
    }


    return ret;
}

pair<vec4, vec4> Cone::getBounds(){
    return {vec4(-1,0,-1,1),vec4(1)};
}

void Cone::draw(mat4 M, unsigned int nodeId){


}

Cone::~Cone(){

}


PrimitiveCollisions Cylinder::Collide(  glm::vec4 E,glm::vec4 P, glm::mat4 M ){
    PrimitiveCollisions ret;
    mat4 invM = inverse(M);
    vec4 invE = invM * E;
    vec4 invP = invM * P;


	double A = pow(invP.x,2) + pow(invP.z,2);
	double B = 2 * (invE.x * invP.x + invE.z * invP.z);
	double C = pow(invE.x,2) + pow(invE.z,2) - 1;
	double roots[2];

	size_t result = quadraticRoots(A, B, C, roots);
    //cout << result << endl;
    if (result < 1)
        return ret;

    vec4 center = vec4(0,0,0,0);

    for (int i = 0; i < result; i++){
        vec4 ourPoint = (invE + roots[i] * invP);
        if (ourPoint.y >= -1 && ourPoint.y <= 1 ){
            CollisionInfo temp = CollisionInfo(roots[i], E + roots[i] * P, normalize(vec4(transpose(inverse(mat3(M))) * vec3(ourPoint.x, -1, ourPoint.z), 0)));
            if (texture != NULL && texture->isValid()){
                temp.useTexture = true;
                vec4 normal = normalize(ourPoint - center);
                temp.kd = texture->getColour(0.5 + atan(normal.z, normal.x) / (pi<float>() * 2), 0.5 - asin(normal.y) / pi<float>());
            }
            ret.addCollision(temp);
        }
    }
    vec3 m_pos = vec3(0,1,0);

    float ty1 = (m_pos.y - invE.y)/invP.y;
    vec4 ourPoint = (invE + ty1 * invP);
    if (pow(ourPoint.x, 2) + pow(ourPoint.z, 2) <= 1){
        CollisionInfo temp = CollisionInfo(ty1, E + ty1 * P, normalize(vec4(transpose(inverse(mat3(M))) * vec3(0, 1, 0), 0)));
        if (texture != NULL && texture->isValid()){
            temp.useTexture = true;
            vec4 normal = normalize(invE + ty1 * invP - center);
            temp.kd = texture->getColour(0.5 + atan(normal.z, normal.x) / (pi<float>() * 2), 0.5 - asin(normal.y) / pi<float>());
        }
        ret.addCollision(temp);

    }

    m_pos = vec3(0,-1,0);
    ty1 = (m_pos.y - invE.y)/invP.y;
    ourPoint = (invE + ty1 * invP);
    if (pow(ourPoint.x, 2) + pow(ourPoint.z, 2) <= 1){
        CollisionInfo temp = CollisionInfo(ty1, E + ty1 * P, normalize(vec4(transpose(inverse(mat3(M))) * vec3(0, -1, 0), 0)));
        if (texture != NULL && texture->isValid()){
            temp.useTexture = true;
            vec4 normal = normalize(invE + ty1 * invP - center);
            temp.kd = texture->getColour(0.5 + atan(normal.z, normal.x) / (pi<float>() * 2), 0.5 - asin(normal.y) / pi<float>());
        }
        ret.addCollision(temp);
    }

    return ret;
}

pair<vec4, vec4> Cylinder::getBounds(){
    return {vec4(-1,-1,-1,1),vec4(1)};
}

void Cylinder::draw(mat4 M, unsigned int nodeId){


}

Cylinder::~Cylinder(){

}

PrimitiveCollisions Torus::Collide(  glm::vec4 E,glm::vec4 P, glm::mat4 M ){
  PrimitiveCollisions ret;
  mat4 invM = inverse(M);
  vec4 invE = invM * E;
  vec4 invP = invM * P;

  const double T = 4.0 * outerRadius * outerRadius;
  const double G = T * (invP.x * invP.x + invP.z * invP.z);
  const double H = 2.0 * T * (invE.x * invP.x + invE.z * invP.z);
  const double I = T * (invE.x * invE.x + invE.z * invE.z);
  const double J = length2(invP);
  const double K = 2.0 * dot(invE, invP);
  const double L = length2(invE) + outerRadius*outerRadius - innerRadius*innerRadius;
  const double JJ = J * J;

	double roots[4];
  // cout << "T: " << T << endl;
  // cout << "G: " << G << endl;
  // cout << "H: " << H << endl;
  // cout << "I: " << I << endl;
  // cout << "J: " << J << endl;
  // cout << "K: " << K << endl;
  // cout << "L: " << L << endl;
	size_t result = quarticRoots(2.0 * K / J, (2.0 * J * L + K * K - G) / JJ, (2.0 * K * L - H) / JJ, (L * L - I) / JJ, roots);
  if (result < 1)
      return ret;

  vec4 center = vec4(0,0,0,1);

  for (int i = 0; i < result; i++){
      // cout << roots[i] << endl;
      // cout << E + roots[i] * P << endl;
      if (roots[i] <= 0) continue;
      vec4 ourPoint = (invE + roots[i] * invP);
      vec3 Q = normalize(vec3(ourPoint.x,0, ourPoint.z)) * outerRadius;
      vec3 normal = normalize(vec3(ourPoint) - Q);
      CollisionInfo temp = CollisionInfo(roots[i], E + roots[i] * P, normalize(vec4(transpose(inverse(mat3(M))) * normal, 0)));



      if (texture != NULL && texture->isValid()){
          temp.useTexture = true;
          vec4 normal = normalize(ourPoint - center);
          temp.kd = texture->getColour(0.5 + atan(normal.z, normal.x) / (pi<float>() * 2), 0.5 - asin(normal.y) / pi<float>());
      }
      ret.addCollision(temp);
  }


  return ret;
}


pair<vec4, vec4> Torus::getBounds(){
    return {vec4(-innerRadius-outerRadius,-innerRadius-outerRadius,-innerRadius,1),vec4(innerRadius+outerRadius,innerRadius+outerRadius,innerRadius,1)};
}

void Torus::draw(mat4 M, unsigned int nodeId){


}

Torus::~Torus(){

}
