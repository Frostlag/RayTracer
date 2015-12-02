#include "SceneNode.hpp"

#include "cs488-framework/MathUtils.hpp"

#include <iostream>

#include <sstream>
using namespace std;

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/component_wise.hpp>
using namespace glm;


// Static class variable
unsigned int SceneNode::nodeInstanceCount = 0;


//---------------------------------------------------------------------------------------
SceneNode::SceneNode(const std::string& name)
  : m_name(name),
	m_nodeType(NodeType::SceneNode),
	trans(mat4()),
	invtrans(mat4()),
	m_nodeId(nodeInstanceCount++)
{
    boundingVolume = NULL;
}

//---------------------------------------------------------------------------------------
// Deep copy
SceneNode::SceneNode(const SceneNode & other)
	: m_nodeType(other.m_nodeType),
	  m_name(other.m_name),
	  trans(other.trans),
	  invtrans(other.invtrans)
{
	for(SceneNode * child : other.children) {
		this->children.push_front(new SceneNode(*child));
	}
}

//---------------------------------------------------------------------------------------
SceneNode::~SceneNode() {
	for(SceneNode * child : children) {
		delete child;
	}
    if ( boundingVolume != NULL)
        delete boundingVolume;
}

//---------------------------------------------------------------------------------------
void SceneNode::set_transform(const glm::mat4& m) {
	trans = m;
	invtrans = glm::inverse(m);
}

//---------------------------------------------------------------------------------------
const glm::mat4& SceneNode::get_transform() const {
	return trans;
}

//---------------------------------------------------------------------------------------
const glm::mat4& SceneNode::get_inverse() const {
	return invtrans;
}

//---------------------------------------------------------------------------------------
void SceneNode::add_child(SceneNode* child) {
	children.push_back(child);

}

//---------------------------------------------------------------------------------------
void SceneNode::rotate(char axis, float angle) {
	vec3 rot_axis;

	switch (axis) {
		case 'x':
			rot_axis = vec3(1,0,0);
			break;
		case 'y':
			rot_axis = vec3(0,1,0);
	        break;
		case 'z':
			rot_axis = vec3(0,0,1);
	        break;
		default:
			break;
	}
	mat4 rot_matrix = glm::rotate(degreesToRadians(angle), rot_axis);
	set_transform( rot_matrix * trans );
}

//---------------------------------------------------------------------------------------
void SceneNode::scale(const glm::vec3 & amount) {
	set_transform( glm::scale(amount) * trans );
}

//---------------------------------------------------------------------------------------
void SceneNode::translate(const glm::vec3& amount) {
	set_transform( glm::translate(amount) * trans );
}


//---------------------------------------------------------------------------------------
int SceneNode::totalSceneNodes() const {
	return nodeInstanceCount;
}

//---------------------------------------------------------------------------------------
std::ostream & operator << (std::ostream & os, const SceneNode & node) {

	//os << "SceneNode:[NodeType: ___, name: ____, id: ____, isSelected: ____, transform: ____"
	switch (node.m_nodeType) {
		case NodeType::SceneNode:
			os << "SceneNode";
			break;
		case NodeType::GeometryNode:
			os << "GeometryNode";
			break;
		case NodeType::JointNode:
			os << "JointNode";
			break;
	}
	os << ":[";

	os << "name:" << node.m_name << ", ";
	os << "id:" << node.m_nodeId;

	os << "]\n";
	return os;
}

void SceneNode::reboundVolume(){

    vec4 min, max;
    bool init = false;
    for( SceneNode * child : children ){
        child->reboundVolume();
		if (child->boundingVolume == NULL) continue;

        mat4 ct = child->get_transform();
        vec4 point1, point2, center;
		NonhierBox* cbv = static_cast<NonhierBox*>(child->boundingVolume);
        float radius = cbv->getSize();
        radius = length(ct * vec4( radius, radius, radius, 0));
        center = ct * vec4(cbv->getPos(),1);

        point1 = center - vec4(radius,radius,radius,0);
        point2 = center + vec4(radius,radius,radius,0);

		if (!init){
            min = glm::min(point1, point2);
            max = glm::max(point1, point2);
		}else{
            min = glm::min(min, point1, point2);
            max = glm::max(max, point1, point2);
		}
		init = true;

    }
    if (init){
        if (boundingVolume != NULL) delete boundingVolume;
        vec4 diff = max-min;
        float diffmax = glm::max(diff.x,diff.y,diff.z);
        boundingVolume = new NonhierBox(vec3(min) - vec3(1), diffmax + 1);
        //cout << m_name << " box bounds: " << endl << to_string(min)  << endl << to_string(max) << " " << diffmax << endl;
    }else{
        if (boundingVolume != NULL) delete boundingVolume;
		boundingVolume = NULL;
	}
}

bool SceneNode::BoundingVolumeCollide(vec4 E, vec4 P, mat4 M){
	if (boundingVolume == NULL) return false;
	return boundingVolume->Bounds(E, P, M * get_transform());

}

PrimitiveCollisions SceneNode::Collide(vec4 E, vec4 P, mat4 M){
	PrimitiveCollisions ret;

	return ret;
}

void SceneNode::draw(mat4 M){

    for(SceneNode* child: children){
        child->draw(M * get_transform());
    }
}
