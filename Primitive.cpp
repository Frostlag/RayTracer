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

    double A = length2(invP);
	double B = 2 * (invP.x * diff.x + invP.y * diff.y + invP.z * diff.z);
	double C = length2(diff) - 1;
	double roots[2];

	size_t result = quadraticRoots(A, B, C, roots);
	if (result < 1)
		return ret;

	double t;
	if (result == 1){
		if (roots[0] < 0)
			return ret;
		t = roots[0];
	}else{
		int i = 3;
		if (roots[0] < 0)
			i -= 1;
		if (roots[1] < 0)
			i -= 2;

		if (i == 3){
			t = glm::min(roots[0],roots[1]);
		}else if(i == 2){
			t = roots[1];
		}else if(i == 1){
			t = roots[0];
		}else if(i == 0)
			return ret;
	}

	float d = t;
	ret.isValid = true;
	ret.position = E + t * P;
	ret.d = t;
    ret.normal = normalize(vec4(transpose(inverse(mat3(M))) * vec3(invE + ret.d * invP), 0));

	return ret;

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

    if (tmin > tmax){

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

	double A = length2(invP);
	double B = 2 * (invP.x * diff.x + invP.y * diff.y + invP.z * diff.z);
	double C = length2(diff) - pow(m_radius,2);
	double roots[2];

	size_t result = quadraticRoots(A, B, C, roots);

	if (result < 1)
		return ret;

	double t;
	if (result == 1){
		if (roots[0] < 0)
			return ret;
		t = roots[0];
	}else{
		int i = 3;
		if (roots[0] < 0)
			i -= 1;
		if (roots[1] < 0)
			i -= 2;

		if (i == 3){
			t = glm::min(roots[0],roots[1]);
		}else if(i == 2){
			t = roots[1];
		}else if(i == 1){
			t = roots[0];
		}else if(i == 0)
			return ret;
	}

	float d = t;
	ret.isValid = true;
	ret.position = E + t * P;
	ret.d = d;
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

    if (tmin > tmax){

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

bool NonhierBox::Bounds(vec4 E, vec4 P, glm::mat4 M){

    CollisionInfo ret;
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

    return tmax >= tmin;

}


pair<glm::vec4,glm::vec4> NonhierBox::getBounds(){
	return {vec4(m_pos,1),vec4(m_pos +  vec3(m_size),1)};
}

NonhierBox::~NonhierBox()
{
}


CollisionInfo Cone::Collide(glm::vec4 E,glm::vec4 P, glm::mat4 M){
    CollisionInfo ret;
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

    double t;
    if (result == 1){
        if (roots[0] < 0)
            return ret;
        t = roots[0];
    }else{
        int i = 3;
        if (roots[0] < 0)
            i -= 1;
        if (roots[1] < 0)
            i -= 2;

        if (i == 3){
            t = glm::min(roots[0],roots[1]);
        }else if(i == 2){
            t = roots[1];
        }else if(i == 1){
            t = roots[0];
        }else if(i == 0)
            return ret;
    }
    vec4 ourPoint = (invE + t * invP);
    if (ourPoint.y >= 0 && ourPoint.y <= 1 ){
        float d = t;
        ret.isValid = true;
        ret.position = E + t * P;
        ret.d = d;
        ret.normal = normalize(vec4(transpose(inverse(mat3(M))) * vec3(ourPoint.x, -1, ourPoint.z), 0));
    }



    return ret;
}

pair<vec4, vec4> Cone::getBounds(){
    return {vec4(-1,0,-1,1),vec4(1)};
}

Cone::~Cone(){

}


CollisionInfo Cylinder::Collide(  glm::vec4 E,glm::vec4 P, glm::mat4 M ){
    CollisionInfo ret;
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

    double t;
    if (result == 1){
        if (roots[0] < 0)
            return ret;
        t = roots[0];
    }else{
        int i = 3;
        if (roots[0] < 0)
            i -= 1;
        if (roots[1] < 0)
            i -= 2;

        if (i == 3){
            t = glm::min(roots[0],roots[1]);
        }else if(i == 2){
            t = roots[1];
        }else if(i == 1){
            t = roots[0];
        }else if(i == 0)
            return ret;
    }

    vec4 ourPoint = (invE + t * invP);
    if (ourPoint.y >= -1 && ourPoint.y <= 1){
        cout << ourPoint.y << endl;
        float d = t;
        ret.isValid = true;
        ret.position = E + t * P;
        ret.d = d;
        ret.normal = normalize(vec4(transpose(inverse(mat3(M))) * vec3(ourPoint.x, 0, ourPoint.z), 0));
    }

    return ret;
}

pair<vec4, vec4> Cylinder::getBounds(){
    return {vec4(-1,-1,-1,1),vec4(1)};
}

Cylinder::~Cylinder(){

}
