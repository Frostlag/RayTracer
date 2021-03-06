#pragma once
#include <vector>
#include <string>
#include "PhongMaterial.hpp"

struct CollisionInfo{
  glm::vec4 position;
  glm::vec4 normal;
  float d;
  bool useTexture;
  glm::vec3 kd;
  CollisionInfo(float d, glm::vec4 position, glm::vec4 normal);
};

struct PrimitiveCollisions{
    bool sorted = false;
    PhongMaterial* mat;
    std::string node_name;
    unsigned int node_id;
    void addCollision(CollisionInfo collisionInfo);
    std::vector<CollisionInfo> getCollisions();
    bool isEmpty(){ return collisions.empty();}
private:
    std::vector<CollisionInfo> collisions;
};
