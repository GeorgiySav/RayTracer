#pragma once

#include <glbinding/gl/gl.h>

#include "objects.h"
#include "camera.h"

namespace engine {
	/// <summary>
	/// Stores all the vertices, mesh info of all objects/entities in a scene
	/// </summary>
	class Scene {
	public:
		Scene();
		~Scene();

		void update();

		void addCircle();

		void setHwAspect(float ratio) { m_camera.setHwAspect(ratio); }
		const Camera& getCamera() const { return m_camera; }

	private:
		Camera m_camera;

		struct SSBO {
			gl::GLuint id;
			gl::GLsizei size;
			gl::GLsizei max_size;
			~SSBO() { gl::glDeleteBuffers(1, &id); }
		};
		SSBO m_circle_ssbo;
	};
}