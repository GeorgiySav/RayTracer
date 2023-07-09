#include "app.h"

#include <glbinding/glbinding.h>
#include <glbinding/getProcAddress.h>

Application::Application() {
	m_window.create({ 960, 540 }, "Ray tracing project");
	m_window.setFramerateLimit(144);
	m_window.setActive(true);
	
	glbinding::initialize(glbinding::getProcAddress, false);

	gl::glViewport(0, 0, m_window.getSize().x, m_window.getSize().y);

	m_scene = std::make_unique<engine::Scene>();
	m_scene->setHwAspect((float)m_window.getSize().y / (float)m_window.getSize().x);

	m_renderer = std::make_unique<engine::Renderer>();
	m_renderer->create(m_window.getSize().x, m_window.getSize().y);

	int work_grp_cnt[3];
	gl::glGetIntegeri_v(gl::GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
	gl::glGetIntegeri_v(gl::GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
	gl::glGetIntegeri_v(gl::GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
	std::cout << "Max work groups per compute shader" <<
		" x:" << work_grp_cnt[0] <<
		" y:" << work_grp_cnt[1] <<
		" z:" << work_grp_cnt[2] << "\n";

	int work_grp_size[3];
	gl::glGetIntegeri_v(gl::GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
	gl::glGetIntegeri_v(gl::GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
	gl::glGetIntegeri_v(gl::GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);
	std::cout << "Max work group sizes" <<
		" x:" << work_grp_size[0] <<
		" y:" << work_grp_size[1] <<
		" z:" << work_grp_size[2] << "\n";

	int work_grp_inv;
	gl::glGetIntegerv(gl::GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
	std::cout << "Max invocations count per work group: " << work_grp_inv << "\n";

	if (m_debug_font.loadFromFile("../../../assets/fonts/W95FA.otf")) {
		m_debug_text.setFont(m_debug_font);
		m_debug_text.setCharacterSize(24);
		m_debug_text.setFillColor(sf::Color(255, 255, 255, 255));
	}

	//m_window.setMouseCursorVisible(false);
	//m_window.setMouseCursorGrabbed(true);
}

Application::~Application() {

}

void Application::run() {
	while (m_window.isOpen()) {
		for (auto event = sf::Event{}; m_window.pollEvent(event);) {
			this->processEvent(event);
		}

		this->update();
		this->render();
	}
}

void Application::update() {
	if (m_window.hasFocus()) {
		m_scene->processMouseMovement(m_window.getPosition().x + int(m_window.getSize().x / 2), m_window.getPosition().y + int(m_window.getSize().y / 2));
		sf::Mouse::setPosition(sf::Vector2i{ m_window.getPosition().x + int(m_window.getSize().x / 2), m_window.getPosition().y + int(m_window.getSize().y / 2) });
	}
	m_scene->update();
}

void Application::render() {
	m_renderer->render(m_scene);

	const engine::Camera& camera = m_scene->getCamera();
	m_debug_text.setString("Position: " + std::to_string(camera.getPosition().x) + ", " + std::to_string(camera.getPosition().y) + ", " + std::to_string(camera.getPosition().z) 
		+ "\nRotation: " + std::to_string(camera.getRotation().x) + ", " + std::to_string(camera.getRotation().y) + ", " + std::to_string(camera.getRotation().z));

	m_window.pushGLStates();
	m_window.draw(m_debug_text);
	m_window.popGLStates();

	m_window.display();
}

void Application::processEvent(sf::Event& e) {
	if (e.type == sf::Event::Closed) {
		m_window.close();
	}
	else if (e.type == sf::Event::Resized) {
		m_renderer->resize(m_window.getSize().x, m_window.getSize().y);
		m_scene->setHwAspect((float)m_window.getSize().y / (float)m_window.getSize().x);
		gl::glViewport(0, 0, m_window.getSize().x, m_window.getSize().y);
	}
	else if (e.type == sf::Event::KeyPressed) {
		if (e.key.code == sf::Keyboard::Escape)
			m_window.close();
		m_scene->processKeyPress(e);
	}
	else if (e.type == sf::Event::MouseMoved) {
		//m_scene->processMouseMovement(e);
	}
}
