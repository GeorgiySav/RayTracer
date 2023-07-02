#include "scene.h"

namespace engine {
	Scene::Scene() : m_camera() {
		m_circle_ssbo.max_size = 10;
		// initialise circle SSBO
		gl::glGenBuffers(1, &m_circle_ssbo.id);
		gl::glBindBuffer(gl::GL_SHADER_STORAGE_BUFFER, m_circle_ssbo.id);
		gl::glBufferData(gl::GL_SHADER_STORAGE_BUFFER, sizeof(Circle) * m_circle_ssbo.max_size, NULL, gl::GL_DYNAMIC_READ);
		gl::glBindBufferBase(gl::GL_SHADER_STORAGE_BUFFER, 1, m_circle_ssbo.id);
		gl::glBindBuffer(gl::GL_SHADER_STORAGE_BUFFER, 0); // unbind
		m_circle_ssbo.size = 0;
		m_circle_ssbo.item_size = sizeof(Circle);
		Circle test = {};
		test.pos = { 0.0, 0.0, 5.0 };
		test.radius = 1.0;
		m_circle_ssbo.add(test);
	}

	Scene::~Scene() {
	}

	void Scene::update() {
		m_camera.update();
	}

	void Scene::addCircle() {
	}
}