#include "scene.h"

namespace engine {
	Scene::Scene() {
		m_circle_ssbo.max_size = 10;
		// initialise circle SSBO
		gl::glGenBuffers(1, &m_circle_ssbo.id);
		gl::glBindBuffer(gl::GL_SHADER_STORAGE_BUFFER, m_circle_ssbo.id);
		gl::glBufferData(gl::GL_SHADER_STORAGE_BUFFER, sizeof(Circle) * m_circle_ssbo.max_size, NULL, gl::GL_READ_WRITE);
	}
}