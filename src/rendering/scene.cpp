#include "scene.h"

namespace engine {
	Scene::Scene() : m_camera() {
		// initialise circle SSBO
		m_circle_ssbo.max_size = 10;
		gl::glGenBuffers(1, &m_circle_ssbo.id);
		gl::glBindBuffer(gl::GL_SHADER_STORAGE_BUFFER, m_circle_ssbo.id);
		gl::glBufferData(gl::GL_SHADER_STORAGE_BUFFER, sizeof(Circle) * m_circle_ssbo.max_size, NULL, gl::GL_DYNAMIC_READ);
		gl::glBindBufferBase(gl::GL_SHADER_STORAGE_BUFFER, 1, m_circle_ssbo.id);
		gl::glBindBuffer(gl::GL_SHADER_STORAGE_BUFFER, 0); // unbind
		m_circle_ssbo.size = 0;
		m_circle_ssbo.item_size = sizeof(Circle);
		Circle test = {};
		test.pos = { 0.0, 0.0, -1.0 };
		test.radius = 0.5;
		m_circle_ssbo.add(test);
		Circle test2 = {};
		test2.pos = { 1.0, 0.0, 4.0 };
		test2.radius = 0.5;
		m_circle_ssbo.add(test2);
		Circle test3 = {};
		test3.pos = { 0.0, 1.0, 4.0 };
		test3.radius = 0.5;
		m_circle_ssbo.add(test3);
	}

	Scene::~Scene() {
	}

	void Scene::update() {
		this->processKeyHolds();
		m_camera.update();
	}
	void Scene::processKeyHolds() {
		float move_offset = 0.01;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			m_camera.move(Direction::FRONT, move_offset);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			m_camera.move(Direction::LEFT, move_offset);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			m_camera.move(Direction::BACK, move_offset);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			m_camera.move(Direction::RIGHT, move_offset);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			m_camera.move(Direction::UP, move_offset);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			m_camera.move(Direction::DOWN, move_offset);
		}
	}

	void Scene::processKeyPress(sf::Event& e) {
		auto code = e.key.code;
		float offset = 0.01;
		
		if (code == sf::Keyboard::W) {
			m_camera.move(Direction::FRONT, offset);
		}
		else if (code == sf::Keyboard::A) {
			m_camera.move(Direction::LEFT, offset);
		}
		else if (code == sf::Keyboard::S) {
			m_camera.move(Direction::BACK, offset);
		}
		else if (code == sf::Keyboard::D) {
			m_camera.move(Direction::RIGHT, offset);
		}
		else if (code == sf::Keyboard::E) {
			m_camera.move(Direction::UP, offset);
		}
		else if (code == sf::Keyboard::Q) {
			m_camera.move(Direction::DOWN, offset);
		}
	}

	void Scene::processMouseMovement(int middle_x, int middle_y) {
		auto current_pos = sf::Mouse::getPosition();
		sf::Vector2i delta_pos = current_pos - sf::Vector2i{middle_x, middle_y};
		m_camera.rotate(delta_pos.x, delta_pos.y);
	 }


	void Scene::addCircle() {
	}
}