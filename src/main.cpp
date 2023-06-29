#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <glbinding/getProcAddress.h>

#include "rendering/renderer.h"

int main()
{
    auto window = sf::Window({ 960, 540 }, "CMake SFML Project" );
    window.setFramerateLimit(144);

    // activate the window
    window.setActive(true);

    glbinding::initialize(glbinding::getProcAddress, false);

	gl::glViewport(0, 0, window.getSize().x, window.getSize().y);

	engine::Renderer renderer;
	renderer.create(window.getSize().x, window.getSize().y);

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

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
			else if (event.type == sf::Event::Resized) {
				renderer.create(window.getSize().x, window.getSize().y);
				gl::glViewport(0, 0, window.getSize().x, window.getSize().y);
			}
        }
 
		renderer.render();

        window.display();
    }

}
