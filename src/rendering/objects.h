#pragma once

#include <glm/glm.hpp>

namespace engine {
	
	struct Material {
		glm::vec3 albedo;
		float smoothness = 0.0;
		glm::vec3 emissive_colour = glm::vec3(1.0);
		float emissive_strength = 0.0;
		glm::vec3 padding0;
		float refractive_index = 0.0;
	};

	struct Sphere {
		glm::vec3 pos;
		float radius;

		Material material;
	};

	struct Plane {
		glm::vec3 pos;
		float padding0;
		glm::vec3 normal;
		float padding1;

		Material material;
	};

	struct Box {
		glm::vec3 min_corner;
		float padding0;
		glm::vec3 max_corner;
		float padding1;

		Material material;
	};
}