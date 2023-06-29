#pragma once

#include <memory>

#include <SFML/Window.hpp>

#include "rendering/renderer.h"

class Application {
public:
	Application();
	~Application();

	void run();
private:
	sf::Window m_window;

	std::unique_ptr<engine::Renderer> m_renderer;

	void update();
	void render();

	void processEvent(sf::Event& e);

};