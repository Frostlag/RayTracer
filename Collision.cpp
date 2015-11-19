#include <algorithm>
#include "Collision.hpp"

using namespace std;

void PrimitiveCollisions::addCollision(CollisionInfo collisionInfo){
    if (collisionInfo.d < 0)
        return;
    collisions.push_back(collisionInfo);
    sorted = false;
}

vector<CollisionInfo> PrimitiveCollisions::getCollisions(){
    if (!sorted)
        sort(collisions.begin(),collisions.end(), [](CollisionInfo a, CollisionInfo b) -> bool{
            return a.d < b.d;
        });
    sorted = true;
    return collisions;
}


CollisionInfo::CollisionInfo(float d, glm::vec4 position, glm::vec4 normal):position(position), normal(normal), d(d){

}
