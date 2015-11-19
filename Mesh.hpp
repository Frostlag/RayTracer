#pragma once

#include <vector>
#include <iosfwd>
#include <string>

#include <glm/glm.hpp>

#include "Primitive.hpp"

struct Triangle
{
	size_t v1;
	size_t v2;
	size_t v3;

	Triangle( size_t pv1, size_t pv2, size_t pv3 )
		: v1( pv1 )
		, v2( pv2 )
		, v3( pv3 )
	{}
};

// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh( const std::string& fname );
  virtual PrimitiveCollisions Collide( glm::vec4 E, glm::vec4 P, glm::mat4 M);
private:
	std::vector<glm::vec3> m_vertices;
	std::vector<Triangle> m_faces;
    friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
	virtual std::pair<glm::vec4,glm::vec4> getBounds();
};
