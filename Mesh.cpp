#include <iostream>
#include <fstream>
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include "cs488-framework/GlErrorCheck.hpp"
#include <glm/ext.hpp>
#include "Utilities.hpp"

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
	size_t s1, s2, s3, n1, n2, n3, t1, t2, t3;

	std::ifstream ifs( fname.c_str() );
	while( ifs >> code ) {
		//cout << code << endl;
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "vn" ) {
			ifs >> vx >> vy >> vz;
			m_normals.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "vt" ) {
			ifs >> vx >> vy;
			m_textures.push_back( glm::vec2( vx, vy ) );
		} else if( code == "f" ) {
			ifs >> s1;
			ifs.ignore(1,'/');
			ifs >> t1;
			ifs.ignore(1,'/');
			ifs >> n1;

			ifs >> s2;
			ifs.ignore(1,'/');
			ifs >> t2;
			ifs.ignore(1,'/');
			ifs >> n2;

			ifs >> s3;
			ifs.ignore(1,'/');
			ifs >> t3;
			ifs.ignore(1,'/');
			ifs >> n3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1, n1 - 1, n2 - 1, n3 - 1, t1 - 1, t2 - 1, t3 - 1) );
			if (s2 == s3 && s1 == s2)
				throw "ASDSDFSDF";
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


PrimitiveCollisions Mesh::Collide(vec4 E, vec4 P, glm::mat4 M){
    PrimitiveCollisions ret;
	mat4 invM = inverse(M);
	vec4 invE = invM * E;
	vec4 invP = invM * P;

	for( int i  = 0; i < m_faces.size(); i++){
		vec3 v1 = m_vertices[m_faces[i].v1];
		vec3 v2 = m_vertices[m_faces[i].v2];
		vec3 v3 = m_vertices[m_faces[i].v3];
		vec3 n1 = m_normals[m_faces[i].n1];
		vec3 n2 = m_normals[m_faces[i].n2];
		vec3 n3 = m_normals[m_faces[i].n3];
		vec3 normal = normalize(cross(v2 - v1, v3 - v1));
		float d = dot(vec4(v1,1) -  invE, vec4(normal,0)) / dot(invP, vec4(normal,0));
		// if (isnan(d)){
		// 	cout << m_faces[i].v1 << " " << m_faces[i].v2 << " " << m_faces[i].v3 << endl;
		// 	cout << v1 << " " << v2 << " " << v3 << endl;
		// 	cout << v2 - v1 << " " << v3 - v1 << endl;
		// 	cout << cross(v2 - v1, v3 - v1) << endl;
		// }

		vector<vec3> points = {v1, v2, v3};
		vector<vec3> normals = {
			-normalize(cross(v2 - v1, normal)),
			-normalize(cross(v3 - v2, normal)),
			-normalize(cross(v1 - v3, normal))
		};
		if (d <= 0) continue;
		vec3 potentialPoint = vec3(invE + d * invP);

		bool isIn = true;
		float t;
		for ( int j = 0; j < 3; j++){

			t = dot(potentialPoint - points[j], normals[j]);
			if (t < 0 || isnan(t)) {
				isIn = false;
				break;
			}
		}

		if (!isIn)
			continue;

		CollisionInfo temp = CollisionInfo(d, E + d * P, normalize(vec4(transpose(inverse(mat3(M))) * normal, 0)));
		if (texture != NULL && texture->isValid()){
			vec3 f1 = v1 - potentialPoint;
			vec3 f2 = v2 - potentialPoint;
			vec3 f3 = v3 - potentialPoint;
			float a = length(cross(-v2 + v1, -v3 + v1));
			float a1 = length(cross(f2, f3)) / a;
			float a2 = length(cross(f3, f1)) / a;
			float a3 = length(cross(f1, f2)) / a;
			vec2 uv = m_textures[m_faces[i].t1] * a1 + m_textures[m_faces[i].t2] * a2 + m_textures[m_faces[i].t3] * a3;
			temp.normal = normalize( vec4(transpose(inverse(mat3(M))) * (n1 * a1 + n2 * a2 + n3 * a3), 0));
			temp.useTexture = true;
			temp.kd = texture->getColour(uv.x,1-uv.y);

		}

		ret.addCollision(temp);
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

void Mesh::draw(mat4 M, unsigned int nodeId){
	GLuint m_vbo_vertexPositions;
	glGenBuffers(1, &m_vbo_vertexPositions);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(vec3),
		&(m_vertices[0].x), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECK_GL_ERRORS;

}
