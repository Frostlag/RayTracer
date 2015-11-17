#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include "Primitive.hpp"

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
);

CollisionInfo traverseScene(SceneNode * root, glm::vec4 E, glm::vec4 P, glm::mat4 M);

glm::vec3 calculateColour(SceneNode* root, const std::list<Light *> & lights, const glm::vec3 & ambient, CollisionInfo collisionInfo, glm::vec4 E, glm::vec4 P);

glm::vec3 generateBG(int x, int y);
