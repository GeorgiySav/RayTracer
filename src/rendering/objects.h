#pragma once

#include <glm/glm.hpp>

namespace engine {
	
	struct Material {
		glm::vec3 albedo;
		float roughness = 1.0;
		//float metallic = 0.0f;
	};

	struct Sphere {
		glm::vec3 pos;
		float radius;

		Material material;
	};
}