
#include <iostream>
#include <glm/ext.hpp>
#include <vector>
#include "Primitive.hpp"
#include "polyroots.hpp"
#include "Utilities.hpp"

using namespace std;
using namespace glm;

Primitive::~Primitive()
{
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
        ret.addCollision(CollisionInfo(roots[i], E + roots[i] * P, normalize(vec4(transpose(inverse(mat3(M))) * vec3(invE + roots[i] * invP), 0))));
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
            }else{
                //cout << to_string(potentialPoint) << " " << to_string(squareInfo[i].first[j]) << endl;
            }
        }
        if (!isIn)
            continue;

        ret.addCollision(CollisionInfo(d, M * potentialPoint, normalize(vec4(transpose(inverse(mat3(M))) * planeInfo[i].second, 0))));
    }

    if (ret.getCollisions().size() == 1){
        // cout << "Eye was" << E << ", P was " << P << endl;
        // cout << "One collision at " << ret.getCollisions().front().position << endl;
    }
    return ret;
}

pair<glm::vec4,glm::vec4> Cube::getBounds(){
	return {vec4(0,0,0,1), vec4(1,1,1,1)};
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
        ret.addCollision(CollisionInfo(roots[i], E + roots[i] * P, normalize(vec4(transpose(inverse(mat3(M))) * (vec3(invE + roots[i] * invP) - m_pos), 0))));
    }

    return ret;

}

pair<glm::vec4,glm::vec4> NonhierSphere::getBounds(){
	return {vec4(m_pos - vec3(m_radius),1),vec4(m_pos + vec3(m_radius),1)};
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
            }else{
                //cout << to_string(potentialPoint) << " " << to_string(squareInfo[i].first[j]) << endl;
            }
        }
        if (!isIn)
            continue;

        ret.addCollision(CollisionInfo(d, M * potentialPoint, normalize(vec4(transpose(inverse(mat3(M))) * planeInfo[i].second, 0))));
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

    for (int i = 0; i < result; i++){
        vec4 ourPoint = (invE + roots[i] * invP);
        if (ourPoint.y >= 0 && ourPoint.y <= 1 ){
            ret.addCollision(CollisionInfo(roots[i], E + roots[i] * P, normalize(vec4(transpose(inverse(mat3(M))) * vec3(ourPoint.x, -1, ourPoint.z), 0))));
        }
    }

    vec3 m_pos = vec3(0,1,0);
    float ty1 = (m_pos.y - invE.y)/invP.y;
    vec4 ourPoint = (invE + ty1 * invP);
    if (pow(ourPoint.x, 2) + pow(ourPoint.z, 2) <= 1)
        ret.addCollision(CollisionInfo(ty1, E + ty1 * P, normalize(vec4(transpose(inverse(mat3(M))) * vec3(0, 1, 0), 0))));


    return ret;
}

pair<vec4, vec4> Cone::getBounds(){
    return {vec4(-1,0,-1,1),vec4(1)};
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

    for (int i = 0; i < result; i++){
        vec4 ourPoint = (invE + roots[i] * invP);
        if (ourPoint.y >= -1 && ourPoint.y <= 1 ){
            ret.addCollision(CollisionInfo(roots[i], E + roots[i] * P, normalize(vec4(transpose(inverse(mat3(M))) * vec3(ourPoint.x, 0, ourPoint.z), 0))));
        }
    }
    vec3 m_pos = vec3(0,1,0);

    float ty1 = (m_pos.y - invE.y)/invP.y;
    vec4 ourPoint = (invE + ty1 * invP);
    if (pow(ourPoint.x, 2) + pow(ourPoint.z, 2) <= 1)
        ret.addCollision(CollisionInfo(ty1, E + ty1 * P, normalize(vec4(transpose(inverse(mat3(M))) * vec3(0, 1, 0), 0))));

    m_pos = vec3(0,-1,0);
    ty1 = (m_pos.y - invE.y)/invP.y;
    ourPoint = (invE + ty1 * invP);
    if (pow(ourPoint.x, 2) + pow(ourPoint.z, 2) <= 1)
        ret.addCollision(CollisionInfo(ty1, E + ty1 * P, normalize(vec4(transpose(inverse(mat3(M))) * vec3(0, -1, 0), 0))));

    return ret;
}

pair<vec4, vec4> Cylinder::getBounds(){
    return {vec4(-1,-1,-1,1),vec4(1)};
}

Cylinder::~Cylinder(){

}
