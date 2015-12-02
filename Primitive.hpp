#pragma once

#include <glm/glm.hpp>
#include <functional>
#include "Collision.hpp"
#include "Texture.hpp"
class SceneNode;

class Primitive {
protected:
  Primitive();
  Texture *texture;
public:
  virtual PrimitiveCollisions Collide(  glm::vec4 E,glm::vec4 P, glm::mat4 M) = 0;
  virtual std::pair<glm::vec4,glm::vec4> getBounds() = 0;
  virtual ~Primitive();
  virtual void draw(glm::mat4 M, unsigned nodeId) = 0;
  void setTexture(Texture *texture);

};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  virtual PrimitiveCollisions Collide(  glm::vec4 E,glm::vec4 P, glm::mat4 M );
  virtual std::pair<glm::vec4,glm::vec4> getBounds();
  virtual void draw(glm::mat4 M, unsigned nodeId);
};

class Cube : public Primitive {
public:
  virtual PrimitiveCollisions Collide(  glm::vec4 E,glm::vec4 P, glm::mat4 M );
  virtual ~Cube();
  virtual std::pair<glm::vec4,glm::vec4> getBounds();
  virtual void draw(glm::mat4 M, unsigned nodeId);
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual PrimitiveCollisions Collide(  glm::vec4 E,glm::vec4 P, glm::mat4 M );
  virtual ~NonhierSphere();
  virtual std::pair<glm::vec4,glm::vec4> getBounds();
  glm::vec3 getPos(){ return m_pos; }
  double getRadius(){ return m_radius; }
  virtual void draw(glm::mat4 M, unsigned nodeId);

private:
  glm::vec3 m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }
  virtual PrimitiveCollisions Collide(  glm::vec4 E,glm::vec4 P, glm::mat4 M );
  bool Bounds( glm::vec4 E,glm::vec4 P, glm::mat4 M );
  virtual ~NonhierBox();

  glm::vec3 getPos(){ return m_pos; }
  double getSize(){ return m_size; }
  virtual std::pair<glm::vec4,glm::vec4> getBounds();
  virtual void draw(glm::mat4 M, unsigned nodeId);
private:
  glm::vec3 m_pos;
  double m_size;
};

class Cone : public Primitive {
public:
  virtual PrimitiveCollisions Collide(  glm::vec4 E,glm::vec4 P, glm::mat4 M );
  virtual ~Cone();
  virtual std::pair<glm::vec4,glm::vec4> getBounds();
  virtual void draw(glm::mat4 M, unsigned nodeId);
};

class Cylinder: public Primitive {
public:
  virtual PrimitiveCollisions Collide(  glm::vec4 E,glm::vec4 P, glm::mat4 M );
  virtual ~Cylinder();
  virtual std::pair<glm::vec4,glm::vec4> getBounds();
  virtual void draw(glm::mat4 M, unsigned nodeId);
};
