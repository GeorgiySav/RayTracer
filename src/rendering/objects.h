#pragma once

#include <glm/glm.hpp>

namespace engine {
	
	struct Material {
		glm::vec3 albedo;
		float smoothness = 0.0;
		glm::vec3 emissive_colour = glm::vec3(1.0);
		float emissive_strength = 0.0;
	};

	struct Sphere {
		glm::vec3 pos;
		float radius;

		Material material;
	};
}