#include "GeometryNode.hpp"
#include <iostream>
#include <glm/ext.hpp>
using namespace std;
using namespace glm;
//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, Material *mat )
	: SceneNode( name )
	, m_material( mat )
	, m_primitive( prim )
{
	m_nodeType = NodeType::GeometryNode;
}

void GeometryNode::setMaterial( Material *mat )
{
	// Obviously, there's a potential memory leak here.  A good solution
	// would be to use some kind of reference counting, as in the
	// C++ shared_ptr.  But I'm going to punt on that problem here.
	// Why?  Two reasons:
	// (a) In practice we expect the scene to be constructed exactly
	//     once.  There's no reason to believe that materials will be
	//     repeatedly overwritten in a GeometryNode.
	// (b) A ray tracer is a program in which you compute once, and
	//     throw away all your data.  A memory leak won't build up and
	//     crash the program.

	m_material = mat;
}


PrimitiveCollisions GeometryNode::Collide(vec4 E, vec4 P, mat4 M){
	if (m_primitive == NULL) return PrimitiveCollisions();
	PrimitiveCollisions primitiveCollisions = m_primitive->Collide(E, P, M * get_transform());
	if (!primitiveCollisions.isEmpty()){
		primitiveCollisions.mat = static_cast<PhongMaterial*>(m_material);

		primitiveCollisions.node_name = m_name;
		primitiveCollisions.node_id = m_nodeId;
		//cout << "collide on " << m_name << " at " << to_string(collisionInfo.position) << " with d=" << collisionInfo.d << endl;
		//cout << "normal at collision point is " << to_string(collisionInfo.normal) << endl;
	}
	return primitiveCollisions;
}

void GeometryNode::reboundVolume(){
	SceneNode::reboundVolume();
	vec4 min, max;
	pair<vec4,vec4> points = m_primitive->getBounds();
	min = glm::min(points.first, points.second);
	max = glm::max(points.first, points.second);

	if (boundingVolume != NULL){
		min = glm::min(min,vec4(boundingVolume->getPos(), 1));
		max = glm::max(max,vec4(boundingVolume->getPos() + vec3(boundingVolume->getSize()), 1));
		delete boundingVolume;
	}
	vec4 diff = max-min;
	float diffmax = glm::max(diff.x,diff.y,diff.z);
	boundingVolume = new NonhierBox(vec3(min) - vec3(1), diffmax + 1);
}

void GeometryNode::setTexture(Texture *texture){
	if (texture == NULL || !texture->isValid() || m_primitive == NULL)
		return;
	m_primitive->setTexture(texture);

}
