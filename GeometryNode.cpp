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


CollisionInfo GeometryNode::Collide(vec4 E, vec4 P, mat4 M){
	if (m_primitive == NULL) return CollisionInfo();
	CollisionInfo collisionInfo = m_primitive->Collide(E, P, M * get_transform());
	if (collisionInfo.isValid){
		collisionInfo.mat = static_cast<PhongMaterial*>(m_material);

		collisionInfo.node_name = m_name;
		//cout << "collide on " << m_name << " at " << to_string(collisionInfo.position) << " with d=" << collisionInfo.d << endl;
		//cout << "normal at collision point is " << to_string(collisionInfo.normal) << endl;
	}
	return collisionInfo;
}