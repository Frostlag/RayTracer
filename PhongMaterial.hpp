#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"

class PhongMaterial : public Material {
public:
  PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess, double reflectivity = 0);
  glm::vec3 getKD() {return m_kd;}
  glm::vec3 getKS() {return m_ks;}
  double getShininess() {return m_shininess;}
  double getReflectivity() {return reflectivity;}
  virtual ~PhongMaterial();

private:
  glm::vec3 m_kd;
  glm::vec3 m_ks;

  double m_shininess;
  double reflectivity;
};
