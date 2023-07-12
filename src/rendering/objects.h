#pragma once

#include <glm/glm.hpp>

namespace engine {
	
	struct Material {
		glm::vec4 colour;
	};

	struct Sphere {
		glm::vec3 pos;
		float radius;

		Material material;
	};
}