#pragma once
#include "Collision.hpp"
#include "GeometryNode.hpp"
class CSGNode: public GeometryNode{
		
protected:
	SceneNode &n1, &n2;
	CSGNode(const std::string & name, SceneNode &n1, SceneNode &n2);

public:

};

class UnionNode: public CSGNode{

public:
	~UnionNode();
	UnionNode(const std::string & name, SceneNode &n1, SceneNode &n2);
	virtual PrimitiveCollisions Collide(glm::vec4 E, glm::vec4 P, glm::mat4 M);
	virtual void reboundVolume();
};

class ConjunctionNode: public CSGNode{

public:
	~ConjunctionNode();
	ConjunctionNode(const std::string & name, SceneNode &n1, SceneNode &n2);
	virtual PrimitiveCollisions Collide(glm::vec4 E, glm::vec4 P, glm::mat4 M);
	virtual void reboundVolume();
};

class SubtractionNode: public CSGNode{

public:
	~SubtractionNode();
	SubtractionNode(const std::string & name, SceneNode &n1, SceneNode &n2);
	virtual PrimitiveCollisions Collide(glm::vec4 E, glm::vec4 P, glm::mat4 M);
	virtual void reboundVolume();
};

class ExclusiveNode: public CSGNode{

public:
	~ExclusiveNode();
	ExclusiveNode(const std::string & name, SceneNode &n1, SceneNode &n2);
	virtual PrimitiveCollisions Collide(glm::vec4 E, glm::vec4 P, glm::mat4 M);
	virtual void reboundVolume();
};
