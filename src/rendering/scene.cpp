#include "scene.h"

namespace engine {
	Scene::Scene() : m_camera() {
		// initialise circle SSBO
		m_circle_ssbo.max_size = 10;
		m_circle_ssbo.size = 0;
		gl::glGenBuffers(1, &m_circle_ssbo.id);
		gl::glBindBuffer(gl::GL_SHADER_STORAGE_BUFFER, m_circle_ssbo.id);
		gl::glBufferData(gl::GL_SHADER_STORAGE_BUFFER, sizeof(Sphere) * m_circle_ssbo.max_size, NULL, gl::GL_DYNAMIC_READ);
		gl::glBindBufferBase(gl::GL_SHADER_STORAGE_BUFFER, 1, m_circle_ssbo.id);
		gl::glBindBuffer(gl::GL_SHADER_STORAGE_BUFFER, 0); // unbind
		Sphere test = {};
		test.pos = { 0.0, 0.5, 1.0 };
		test.radius = 0.5;
		test.material.albedo = { 1.0, 1.0, 1.0 };
		test.material.smoothness = 1.0;
		//test.material.metallic = 0.0;
		m_circle_ssbo.add(test);
		Sphere test2 = {};
		test2.pos = { 1.0, 0.5, 0.0 };
		test2.radius = 0.5;
		test2.material.albedo = { 1.0, 1.0, 1.0 };
		test2.material.smoothness = 0.0;
		m_circle_ssbo.add(test2);
		Sphere test3 = {};
		test3.pos = { 0.0, -5.0, 0.0 };
		test3.radius = 5.0;
		test3.material.albedo = { 0.0, 1.0, 0.0 };
		test3.material.smoothness = 0.5;
		m_circle_ssbo.add(test3);
		Sphere test4 = {};
		test4.pos = { 0.0, 1.0, 0.0 };
		test4.radius = 0.25;
		test4.material.albedo = { 0.5, 0.1, 0.2 };
		test4.material.smoothness = 0.0;
		test4.material.emissive_colour = { 0.5, 0.1, 0.2 };
		test4.material.emissive_strength = 100.0;
		m_circle_ssbo.add(test4);
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			m_camera.move(Direction::LEFT, move_offset);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			m_camera.move(Direction::BACK, move_offset);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			m_camera.move(Direction::RIGHT, move_offset);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			m_camera.move(Direction::UP, move_offset);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			m_camera.move(Direction::DOWN, move_offset);
		}
	}

	void Scene::processKeyPress(sf::Event& e) {
		auto code = e.key.code;
		switch (code)
		{
		case sf::Keyboard::Unknown:
			break;
		case sf::Keyboard::A:
			break;
		case sf::Keyboard::B:
			break;
		case sf::Keyboard::C:
			break;
		case sf::Keyboard::D:
			break;
		case sf::Keyboard::E:
			break;
		case sf::Keyboard::F:
			break;
		case sf::Keyboard::G:
			break;
		case sf::Keyboard::H:
			break;
		case sf::Keyboard::I:
			break;
		case sf::Keyboard::J:
			break;
		case sf::Keyboard::K:
			break;
		case sf::Keyboard::L:
			break;
		case sf::Keyboard::M:
			break;
		case sf::Keyboard::N:
			break;
		case sf::Keyboard::O:
			break;
		case sf::Keyboard::P:
			break;
		case sf::Keyboard::Q:
			break;
		case sf::Keyboard::R:
			break;
		case sf::Keyboard::S:
			break;
		case sf::Keyboard::T:
			break;
		case sf::Keyboard::U:
			break;
		case sf::Keyboard::V:
			break;
		case sf::Keyboard::W:
			break;
		case sf::Keyboard::X:
			break;
		case sf::Keyboard::Y:
			break;
		case sf::Keyboard::Z:
			break;
		case sf::Keyboard::Num0:
			break;
		case sf::Keyboard::Num1:
			break;
		case sf::Keyboard::Num2:
			break;
		case sf::Keyboard::Num3:
			break;
		case sf::Keyboard::Num4:
			break;
		case sf::Keyboard::Num5:
			break;
		case sf::Keyboard::Num6:
			break;
		case sf::Keyboard::Num7:
			break;
		case sf::Keyboard::Num8:
			break;
		case sf::Keyboard::Num9:
			break;
		case sf::Keyboard::Escape:
			break;
		case sf::Keyboard::LControl:
			break;
		case sf::Keyboard::LShift:
			break;
		case sf::Keyboard::LAlt:
			break;
		case sf::Keyboard::LSystem:
			break;
		case sf::Keyboard::RControl:
			break;
		case sf::Keyboard::RShift:
			break;
		case sf::Keyboard::RAlt:
			break;
		case sf::Keyboard::RSystem:
			break;
		case sf::Keyboard::Menu:
			break;
		case sf::Keyboard::LBracket:
			break;
		case sf::Keyboard::RBracket:
			break;
		case sf::Keyboard::Semicolon:
			break;
		case sf::Keyboard::Comma:
			break;
		case sf::Keyboard::Period:
			break;
		case sf::Keyboard::Apostrophe:
			break;
		case sf::Keyboard::Slash:
			break;
		case sf::Keyboard::Backslash:
			break;
		case sf::Keyboard::Grave:
			break;
		case sf::Keyboard::Equal:
			break;
		case sf::Keyboard::Hyphen:
			break;
		case sf::Keyboard::Space:
			break;
		case sf::Keyboard::Enter:
			break;
		case sf::Keyboard::Backspace:
			break;
		case sf::Keyboard::Tab:
			break;
		case sf::Keyboard::PageUp:
			break;
		case sf::Keyboard::PageDown:
			break;
		case sf::Keyboard::End:
			break;
		case sf::Keyboard::Home:
			break;
		case sf::Keyboard::Insert:
			break;
		case sf::Keyboard::Delete:
			break;
		case sf::Keyboard::Add:
			break;
		case sf::Keyboard::Subtract:
			break;
		case sf::Keyboard::Multiply:
			break;
		case sf::Keyboard::Divide:
			break;
		case sf::Keyboard::Left:
			break;
		case sf::Keyboard::Right:
			break;
		case sf::Keyboard::Up:
			break;
		case sf::Keyboard::Down:
			break;
		case sf::Keyboard::Numpad0:
			break;
		case sf::Keyboard::Numpad1:
			break;
		case sf::Keyboard::Numpad2:
			break;
		case sf::Keyboard::Numpad3:
			break;
		case sf::Keyboard::Numpad4:
			break;
		case sf::Keyboard::Numpad5:
			break;
		case sf::Keyboard::Numpad6:
			break;
		case sf::Keyboard::Numpad7:
			break;
		case sf::Keyboard::Numpad8:
			break;
		case sf::Keyboard::Numpad9:
			break;
		case sf::Keyboard::F1:
			break;
		case sf::Keyboard::F2:
			break;
		case sf::Keyboard::F3:
			break;
		case sf::Keyboard::F4:
			break;
		case sf::Keyboard::F5:
			break;
		case sf::Keyboard::F6:
			break;
		case sf::Keyboard::F7:
			break;
		case sf::Keyboard::F8:
			break;
		case sf::Keyboard::F9:
			break;
		case sf::Keyboard::F10:
			break;
		case sf::Keyboard::F11:
			break;
		case sf::Keyboard::F12:
			break;
		case sf::Keyboard::F13:
			break;
		case sf::Keyboard::F14:
			break;
		case sf::Keyboard::F15:
			break;
		case sf::Keyboard::Pause:
			break;
		case sf::Keyboard::KeyCount:
			break;
		default:
			break;
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