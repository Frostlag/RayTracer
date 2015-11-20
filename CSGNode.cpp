#include "CSGNode.hpp"

using namespace std;

CSGNode::CSGNode(const std::string & name, SceneNode &n1, SceneNode &n2)
:GeometryNode(name, nullptr, nullptr), n1(n1), n2(n2){
	m_nodeType = NodeType::CSGNode;
}

UnionNode::UnionNode(const std::string & name, SceneNode& n1, SceneNode& n2):CSGNode(name, n1, n2){

}

PrimitiveCollisions UnionNode::Collide(glm::vec4 E, glm::vec4 P, glm::mat4 M){
	PrimitiveCollisions ret;
	PrimitiveCollisions n1pc = n1.Collide(E, P, M * get_transform());
	PrimitiveCollisions n2pc = n2.Collide(E, P, M * get_transform());
	vector<CollisionInfo> n1c = n1pc.getCollisions();
	vector<CollisionInfo> n2c = n2pc.getCollisions();
	bool inN1 = false, inN2 = false;
	int cs = n1c.size() + n2c.size(), n1counter = 0, n2counter = 0;
	if (n1c.empty()){
		ret = n2pc; 
	}else if (n2c.empty()){
		ret = n1pc;
	}else{
		for (int i = 0; i < cs; i++){
			if (n1counter < n1c.size() && n1c[n1counter].d < n2c[n2counter].d){
				if (!inN2){
					ret.addCollision(n1c[n1counter]);
				}
				inN1 = !inN1;
				n1counter++;
			}else if(n2counter < n2c.size()){
				if (!inN1){
					ret.addCollision(n2c[n2counter]);
				}
				inN2 = !inN2;
				n2counter++;
			}
		}
	}
	if (!ret.isEmpty()){
		ret.mat = static_cast<PhongMaterial*>(m_material);

		ret.node_name = m_name;
	}

	return ret;
}

void UnionNode::reboundVolume(){
	SceneNode::reboundVolume();
}

UnionNode::~UnionNode(){

}

ConjunctionNode::ConjunctionNode(const std::string & name, SceneNode &n1, SceneNode &n2):CSGNode(name, n1, n2){
}

PrimitiveCollisions ConjunctionNode::Collide(glm::vec4 E, glm::vec4 P, glm::mat4 M){
	PrimitiveCollisions ret;
	PrimitiveCollisions n1pc = n1.Collide(E, P, M * get_transform());
	PrimitiveCollisions n2pc = n2.Collide(E, P, M * get_transform());
	vector<CollisionInfo> n1c = n1pc.getCollisions();
	vector<CollisionInfo> n2c = n2pc.getCollisions();
	bool inN1 = false, inN2 = false;
	int cs = n1c.size() + n2c.size(), n1counter = 0, n2counter = 0;
	if (!n1c.empty() && !n2c.empty()){
		for (int i = 0; i < cs; i++){
			if (n1counter < n1c.size() && n1c[n1counter].d < n2c[n2counter].d){
				if (inN2){
					ret.addCollision(n1c[n1counter]);
				}
				inN1 = !inN1;
				n1counter++;
			}else if(n2counter < n2c.size()){
				if (inN1){
					ret.addCollision(n2c[n2counter]);
				}
				inN2 = !inN2;
				n2counter++;
			}
		}
	}
	if (!ret.isEmpty()){
		ret.mat = static_cast<PhongMaterial*>(m_material);

		ret.node_name = m_name;
	}

	return ret;
}

void ConjunctionNode::reboundVolume(){
	SceneNode::reboundVolume();
}

ConjunctionNode::~ConjunctionNode(){
}

SubtractionNode::SubtractionNode(const std::string & name, SceneNode &n1, SceneNode &n2):CSGNode(name, n1, n2){
}

PrimitiveCollisions SubtractionNode::Collide(glm::vec4 E, glm::vec4 P, glm::mat4 M){
	PrimitiveCollisions ret;
	PrimitiveCollisions n1pc = n1.Collide(E, P, M * get_transform());
	PrimitiveCollisions n2pc = n2.Collide(E, P, M * get_transform());
	vector<CollisionInfo> n1c = n1pc.getCollisions();
	vector<CollisionInfo> n2c = n2pc.getCollisions();
	bool inN1 = false, inN2 = false;
	int cs = n1c.size() + n2c.size(), n1counter = 0, n2counter = 0;
	if (n2c.empty())
		ret = n1pc;
	else if (!n1c.empty()){
		glm::vec4 normal;
		for (int i = 0; i < cs; i++){
			if (n1counter < n1c.size() && n1c[n1counter].d < n2c[n2counter].d){
				if (!inN2)
					ret.addCollision(n1c[n1counter]);
				inN1 = !inN1;
				n1counter++;
			}else if(n2counter < n2c.size()){
				if (inN1){
					CollisionInfo collisionInfo = n2c[n2counter];
					collisionInfo.normal *= -1;
					ret.addCollision(collisionInfo);
				}
				inN2 = !inN2;
				n2counter++;
			}
		}
	}
	if (!ret.isEmpty()){
		ret.mat = static_cast<PhongMaterial*>(m_material);

		ret.node_name = m_name;
	}

	return ret;
}

void SubtractionNode::reboundVolume(){
	SceneNode::reboundVolume();
}

SubtractionNode::~SubtractionNode(){

}

ExclusiveNode::ExclusiveNode(const std::string & name, SceneNode &n1, SceneNode &n2):CSGNode(name, n1, n2){
}

PrimitiveCollisions ExclusiveNode::Collide(glm::vec4 E, glm::vec4 P, glm::mat4 M){
	PrimitiveCollisions ret;
	PrimitiveCollisions n1pc = n1.Collide(E, P, M * get_transform());
	PrimitiveCollisions n2pc = n2.Collide(E, P, M * get_transform());
	vector<CollisionInfo> n1c = n1pc.getCollisions();
	vector<CollisionInfo> n2c = n2pc.getCollisions();
	bool inN1 = false, inN2 = false;
	int cs = n1c.size() + n2c.size(), n1counter = 0, n2counter = 0;
	if (n1c.empty()){
		ret = n2pc; 
	}else if (n2c.empty()){
		ret = n1pc;
	}else{
		for (int i = 0; i < cs; i++){
			if (n1counter < n1c.size() && n1c[n1counter].d < n2c[n2counter].d){
				if (!inN2)
					ret.addCollision(n1c[n1counter]);
				inN1 = !inN1;
				n1counter++;
			}else if(n2counter < n2c.size()){
				if (!inN1)
				ret.addCollision(n2c[n2counter]);
				inN2 = !inN2;
				n2counter++;
			}
		}
	}
	if (!ret.isEmpty()){
		ret.mat = static_cast<PhongMaterial*>(m_material);

		ret.node_name = m_name;
	}

	return ret;
}

void ExclusiveNode::reboundVolume(){
	SceneNode::reboundVolume();
}

ExclusiveNode::~ExclusiveNode(){

}
