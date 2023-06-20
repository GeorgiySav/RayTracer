#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <glbinding/getProcAddress.h>

#include "rendering/shader.h"

gl::GLfloat vertices[] =
{
	-1.0f, -1.0f , 0.0f, 0.0f, 0.0f,
	-1.0f,  1.0f , 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f , 0.0f, 1.0f, 1.0f,
	 1.0f, -1.0f , 0.0f, 1.0f, 0.0f,
};

gl::GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2
};

const int SCREEN_WIDTH = 480;
const int SCREEN_HEIGHT = 270;

int main()
{
    auto window = sf::Window({ 960, 540 }, "CMake SFML Project" );
    window.setFramerateLimit(144);

    // activate the window
    window.setActive(true);

    glbinding::initialize(glbinding::getProcAddress, false);

	gl::glViewport(0, 0, window.getSize().x, window.getSize().y);

	gl::GLuint VAO, VBO, EBO;
	gl::glCreateVertexArrays(1, &VAO);
	gl::glCreateBuffers(1, &VBO);
	gl::glCreateBuffers(1, &EBO);

	gl::glNamedBufferData(VBO, sizeof(vertices), vertices, gl::GL_STATIC_DRAW);
	gl::glNamedBufferData(EBO, sizeof(indices), indices, gl::GL_STATIC_DRAW);

	gl::glEnableVertexArrayAttrib(VAO, 0);
	gl::glVertexArrayAttribBinding(VAO, 0, 0);
	gl::glVertexArrayAttribFormat(VAO, 0, 3, gl::GL_FLOAT, gl::GL_FALSE, 0);

	gl::glEnableVertexArrayAttrib(VAO, 1);
	gl::glVertexArrayAttribBinding(VAO, 1, 0);
	gl::glVertexArrayAttribFormat(VAO, 1, 2, gl::GL_FLOAT, gl::GL_FALSE, 3 * sizeof(gl::GLfloat));

	gl::glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(gl::GLfloat));
	gl::glVertexArrayElementBuffer(VAO, EBO);

	gl::GLuint screenTex;
	gl::glCreateTextures(gl::GL_TEXTURE_2D, 1, &screenTex);
	gl::glTextureParameteri(screenTex, gl::GL_TEXTURE_MIN_FILTER, gl::GL_NEAREST);
	gl::glTextureParameteri(screenTex, gl::GL_TEXTURE_MAG_FILTER, gl::GL_NEAREST);
	gl::glTextureParameteri(screenTex, gl::GL_TEXTURE_WRAP_S, gl::GL_CLAMP_TO_EDGE);
	gl::glTextureParameteri(screenTex, gl::GL_TEXTURE_WRAP_T, gl::GL_CLAMP_TO_EDGE);
	gl::glTextureStorage2D(screenTex, 1, gl::GL_RGBA32F, SCREEN_WIDTH, SCREEN_HEIGHT);
	gl::glBindImageTexture(0, screenTex, 0, gl::GL_FALSE, 0, gl::GL_WRITE_ONLY, gl::GL_RGBA32F);

	rendering::VertFragProgram screenShaderProgram = rendering::VertFragProgram("../../../src/shaders/screen_vert.vert", "../../../src/shaders/screen_frag.frag");

	rendering::ComputeProgram computeProgram = rendering::ComputeProgram("../../../src/shaders/ray_tracer.comp");

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
        }

        // clear the buffers
        gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

		computeProgram.use();
		computeProgram.dispatch(std::ceil(SCREEN_WIDTH * 0.125), std::ceil(SCREEN_HEIGHT * 0.125), 1, gl::GL_ALL_BARRIER_BITS);

		screenShaderProgram.use();
		//gl::glBindTextureUnit(0, screenTex);
		screenShaderProgram.setTexture("screen", screenTex);
		gl::glBindVertexArray(VAO);
		gl::glDrawElements(gl::GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), gl::GL_UNSIGNED_INT, 0);

        window.display();
    }

}
