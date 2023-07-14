#pragma once

#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "rendering/renderer.h"

class Application {
public:
	Application();
	~Application();

	void run();
private:
	sf::RenderWindow m_window;

	std::unique_ptr<engine::Renderer> m_renderer;
	std::unique_ptr<engine::Scene> m_scene;

	// debugging
	sf::Font m_debug_font;
	sf::Text m_debug_text;

	float m_fps;
	sf::Clock m_clock = sf::Clock::Clock();
	sf::Time m_previousTime = m_clock.getElapsedTime();
	sf::Time m_currentTime;

	void update();
	void render();

	void processEvent(sf::Event& e);

};