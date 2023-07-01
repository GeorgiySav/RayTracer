#pragma once

#include <glbinding/gl/gl.h>

#include "objects.h"

namespace engine {
	/// <summary>
	/// Stores all the vertices, mesh info of all objects/entities in a scene
	/// </summary>
	class Scene {
	public:
		Scene();
		~Scene();

		void addCircle();

	private:
		struct SSBO {
			gl::GLuint id;
			gl::GLsizei size;
			gl::GLsizei max_size;
			~SSBO() { gl::glDeleteBuffers(1, &id); }
		};
		SSBO m_circle_ssbo;
	};
}