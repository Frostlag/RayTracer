#include "PhongMaterial.hpp"
#include <iostream>
using namespace std;
PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess, double reflectivity)
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
	, reflectivity(reflectivity)

{
}

PhongMaterial::~PhongMaterial()
{}
