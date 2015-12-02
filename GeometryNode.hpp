#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"
#include <glm/glm.hpp>



class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim,
		Material *mat = nullptr );

	void setMaterial( Material *material );
	void setTexture(Texture *texture);

	Material *m_material;
	Primitive *m_primitive;

	virtual PrimitiveCollisions Collide(glm::vec4 E, glm::vec4 P, glm::mat4 M);
	virtual void reboundVolume();
	virtual void draw(glm::mat4 M);
};
