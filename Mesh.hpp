#pragma once

#include <vector>
#include <iosfwd>
#include <string>

#include <glm/glm.hpp>

#include "Primitive.hpp"

struct Triangle
{
	size_t v1, v2, v3;
	size_t n1, n2, n3;
	size_t t1, t2, t3;
	Triangle( size_t pv1, size_t pv2, size_t pv3, size_t pn1, size_t pn2, size_t pn3, size_t pt1, size_t pt2, size_t pt3)
		: v1( pv1 )
		, v2( pv2 )
		, v3( pv3 )
		, n1( pn1 )
		, n2( pn2 )
		, n3( pn3 )
		, t1( pt1 )
		, t2( pt2 )
		, t3( pt3 )
	{}
};

// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh( const std::string& fname );
  virtual PrimitiveCollisions Collide( glm::vec4 E, glm::vec4 P, glm::mat4 M);
  virtual void draw(glm::mat4 M, unsigned int nodeId);
private:
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec2> m_textures;
	std::vector<Triangle> m_faces;
    friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
	virtual std::pair<glm::vec4,glm::vec4> getBounds();
};
