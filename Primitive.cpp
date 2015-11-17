#include "Primitive.hpp"
#include <iostream>
#include <glm/ext.hpp>
#include "polyroots.hpp"
#include <vector>
using namespace std;
using namespace glm;

CollisionInfo::CollisionInfo():isValid(false),mat(NULL){}

Primitive::~Primitive()
{
}

CollisionInfo Sphere::Collide(  vec4 E, vec4 P, glm::mat4 M ){
    CollisionInfo ret;
    vec4 center = vec4(0,0,0,1);
    mat4 invM = inverse(M);
    vec4 invE = invM * E;
    vec4 invP = invM * P;
    vec4 diff = invE - center;
    float invPsq = pow(length(invP),2);

    float discriminant = (pow(dot(invP , diff),2) - invPsq * (dot(diff,diff) - 1));


    if (discriminant < 0){
        return ret;
    }
    
    float otherPart = - dot(invP,diff);
    //cout << "otherPart: " << otherPart << endl;
    //cout << "sqrt discriminant: " << sqrt(discriminant) << endl;

    float d1 = (otherPart + sqrt(discriminant)) / invPsq;
    float d2 = (otherPart - sqrt(discriminant)) / invPsq;
    if (d2 > d1 || d2 < 0){
        if (d1 < 0) return ret;
        ret.isValid = true;
        ret.position = E + (d1 * P);
        ret.d = d1;
    }else if (d2 > 0){
        ret.isValid = true;
        ret.position = E + (d2 * P);
        ret.d = d2;
    }

    ret.normal = normalize(vec4(transpose(inverse(mat3(M))) * vec3(invE + ret.d * invP), 0));
    //cout << ret.d << endl;
    // cout << "Two hits" << endl;



    // cout << pow(dot(P ,(E - center)),2) << endl;
    // cout << pow(length(P - center),2) << endl;
    // cout << m_radius * m_radius << endl;

    return ret;
}

pair<glm::vec4,glm::vec4> Sphere::getBounds(){
	return {vec4(-1,-1,-1,1),vec4(1,1,1,1)};
}


Sphere::~Sphere()
{
}

CollisionInfo Cube::Collide(  vec4 E, vec4 P, glm::mat4 M ){
    CollisionInfo ret;
    mat4 invM = inverse(M);
    vec4 invE = invM * E;
    vec4 invP = invM * P;

    vec3 m_pos = vec3(0);
    float m_size = 1;
    vec3 m_pos2 = m_pos + vec3(m_size,m_size,m_size);
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

        if (!ret.isValid || ret.d > d){
            ret.isValid = true;
            ret.position = M * potentialPoint;
            ret.d = d;
            ret.normal = normalize(vec4(transpose(inverse(mat3(M))) * planeInfo[i].second, 0));
            //cout << to_string(ret.normal) << endl;
            //cout << "Cube" << to_string(potentialPoint) << endl;
        }
    }
    //if (ret.isValid) cout << to_string(E + ret.d * P) << endl;
    return ret;
}

pair<glm::vec4,glm::vec4> Cube::getBounds(){
	return {vec4(0,0,0,1), vec4(1,1,1,1)};
}


Cube::~Cube()
{
}

CollisionInfo NonhierSphere::Collide(  vec4 E, vec4 P, glm::mat4 M ){
    CollisionInfo ret;
    vec4 center = vec4(m_pos,1);
    mat4 invM = inverse(M);
    vec4 invE = invM * E;
    vec4 invP = invM * P;
    vec4 diff = invE - center;
    float invPsq = pow(length(invP),2);

    float discriminant = (pow(dot(invP , diff),2) - invPsq * (dot(diff,diff) - m_radius * m_radius));

	if (discriminant < 0){
        //cout << "No hit" << endl;
        return ret;
    }

	float otherPart = - dot(invP,diff);
    //cout << "otherPart: " << otherPart << endl;
    //cout << "sqrt discriminant: " << sqrt(discriminant) << endl;
    float d1 = (otherPart + sqrt(discriminant)) / invPsq;
    float d2 = (otherPart - sqrt(discriminant)) / invPsq;
	if (d2 > d1 || d2 < 0){
        if (d1 < 0) return ret;
        ret.isValid = true;
		ret.position = E + d1 * P;
		ret.d = d1;
	}else if (d2 > 0){
        ret.isValid = true;
		ret.position = E + d2 * P;
		ret.d = d2;
	}

    ret.normal = normalize(vec4(transpose(inverse(mat3(M))) * (vec3(invE + ret.d * invP) - m_pos), 0));

	return ret;
}

pair<glm::vec4,glm::vec4> NonhierSphere::getBounds(){
	return {vec4(m_pos - vec3(m_radius),1),vec4(m_pos + vec3(m_radius),1)};
}

NonhierSphere::~NonhierSphere()
{
}

CollisionInfo NonhierBox::Collide(vec4 E, vec4 P, glm::mat4 M){

    CollisionInfo ret;
    mat4 invM = inverse(M);
    vec4 invE = invM * E;
    vec4 invP = invM * P;

    vec3 m_pos2 = m_pos + vec3(m_size,m_size,m_size);
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

        if (!ret.isValid || ret.d > d){
            ret.isValid = true;
            ret.position = M * potentialPoint;
            ret.d = d;
            ret.normal = normalize(vec4(transpose(inverse(mat3(M))) * planeInfo[i].second, 0));
            //cout << "Cube" << to_string(potentialPoint) << endl;
        }
    }
    //if (ret.isValid) cout << to_string(E + ret.d * P) << endl;
    return ret;
}

pair<glm::vec4,glm::vec4> NonhierBox::getBounds(){
	return {vec4(m_pos,1),vec4(m_pos +  vec3(m_size),1)};
}

NonhierBox::~NonhierBox()
{
}
