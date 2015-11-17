#include <iostream>
#include <fstream>


#include <glm/ext.hpp>
using namespace glm;
using namespace std;
// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"

Mesh::Mesh( const std::string& fname )
	: m_vertices()
	, m_faces()
{
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;

	std::ifstream ifs( fname.c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  /*

  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
  	const MeshVertex& v = mesh.m_verts[idx];
  	out << glm::to_string( v.m_position );
	if( mesh.m_have_norm ) {
  	  out << " / " << glm::to_string( v.m_normal );
	}
	if( mesh.m_have_uv ) {
  	  out << " / " << glm::to_string( v.m_uv );
	}
  }

*/
  out << "}";
  return out;
}


CollisionInfo Mesh::Collide(vec4 E, vec4 P, glm::mat4 M){
    CollisionInfo ret;
	mat4 invM = inverse(M);
	vec4 invE = invM * E;
	vec4 invP = invM * P;

	for( int i  = 0; i < m_faces.size(); i++){
		vec3 v1 = m_vertices[m_faces[i].v1];
		vec3 v2 = m_vertices[m_faces[i].v2];
		vec3 v3 = m_vertices[m_faces[i].v3];
		vec3 normal = normalize(cross(v2 - v1, v3 - v1));
		float d = dot(vec4(v1,1) -  invE, vec4(normal,0)) / dot(invP, vec4(normal,0));
		vector<vec3> points = {v1, v2, v3};
		vector<vec3> normals = {
			-normalize(cross(v2 - v1, normal)),
			-normalize(cross(v3 - v2, normal)),
			-normalize(cross(v1 - v3, normal))
		};
		if (d <= 0) continue;
		vec4 potentialPoint = invE + d * invP;
		bool isIn = true;
		for ( int j = 0; j < 3; j++){
			float t = dot(vec3(potentialPoint) - points[j], normals[j]);
			if (t < 0) {
				isIn = false;
				break;
			}else{
				//cout << to_string(potentialPoint) << " " << to_string(squareInfo[i].first[j]) << endl;
			}
		}
		if (!isIn)
			continue;
		//std::cout << "IN FACE" << std::endl;
		if (!ret.isValid || ret.d > d){
			//std::cout << "On top IN FACE" << std::endl;
			ret.isValid = true;
			ret.d = d;
			ret.position = E + d * P;

			ret.normal = normalize(vec4(transpose(inverse(mat3(M))) * normal, 0));
			//cout << to_string(potentialPoint) << endl;
		}
	}
	return ret;
}

pair<glm::vec4,glm::vec4> Mesh::getBounds(){
	pair<vec4,vec4> ret;
	if (m_vertices.size() > 0){
		ret = {vec4(m_vertices[0],1),vec4(m_vertices[0],1)};
	}
	for (int i = 0; i < m_vertices.size(); i++){
		ret = {glm::min(ret.first,vec4(m_vertices[i],1)),glm::max(ret.second,vec4(m_vertices[i],1))};
	}
	return ret;
}
