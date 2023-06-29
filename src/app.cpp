#include "app.h"

#include <glbinding/glbinding.h>
#include <glbinding/getProcAddress.h>

Application::Application() {
	m_window.create({ 960, 540 }, "Ray tracing project");
	m_window.setFramerateLimit(144);
	m_window.setActive(true);
	
	glbinding::initialize(glbinding::getProcAddress, false);
	gl::glViewport(0, 0, m_window.getSize().x, m_window.getSize().y);

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
}

void Application::render() {
	m_renderer->render();
	m_window.display();
}

void Application::processEvent(sf::Event& e) {
	if (e.type == sf::Event::Closed) {
		m_window.close();
	}
	else if (e.type == sf::Event::Resized) {
		m_renderer->resize(m_window.getSize().x, m_window.getSize().y);
		gl::glViewport(0, 0, m_window.getSize().x, m_window.getSize().y);
	}
}
