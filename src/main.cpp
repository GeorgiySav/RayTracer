#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <glbinding/getProcAddress.h>

#include "rendering/shader_utility.h"

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


const char* screenVertexShaderSource = R"(#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uvs;
out vec2 UVs;
void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
	UVs = uvs;
})";
const char* screenFragmentShaderSource = R"(#version 460 core
out vec4 FragColor;
uniform sampler2D screen;
in vec2 UVs;
void main()
{
	FragColor = texture(screen, UVs);
})";
const char* screenComputeShaderSource = R"(#version 460 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D screen;
void main()
{
	vec4 pixel = vec4(0.075, 0.133, 0.173, 1.0);
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	
	ivec2 dims = imageSize(screen);
	float x = -(float(pixel_coords.x * 2 - dims.x) / dims.x); // transforms to [-1.0, 1.0]
	float y = -(float(pixel_coords.y * 2 - dims.y) / dims.y); // transforms to [-1.0, 1.0]

	float fov = 90.0;
	vec3 cam_o = vec3(0.0, 0.0, -tan(fov / 2.0));
	vec3 ray_o = vec3(x, y, 0.0);
	vec3 ray_d = normalize(ray_o - cam_o);

	vec3 sphere_c = vec3(0.0, 0.0, -5.0);
	float sphere_r = 1.0;

	vec3 o_c = ray_o - sphere_c;
	float b = dot(ray_d, o_c);
	float c = dot(o_c, o_c) - sphere_r * sphere_r;
	float intersectionState = b * b - c;
	vec3 intersection = ray_o + ray_d * (-b + sqrt(b * b - c));

	if (intersectionState >= 0.0)
	{
		pixel = vec4((normalize(intersection - sphere_c) + 1.0) / 2.0, 1.0);
	}

	imageStore(screen, pixel_coords, pixel);
})";

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

	gl::GLuint screenShaderProgram = rendering::createVertFragProgram("../../../src/shaders/screen_vert.vert", "../../../src/shaders/screen_frag.frag");

	gl::GLuint computeProgram = rendering::createComputeProgram("../../../src/shaders/ray_tracer.comp");

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

		gl::glUseProgram(computeProgram);
		gl::glDispatchCompute(std::ceil(SCREEN_WIDTH * 0.125), std::ceil(SCREEN_HEIGHT * 0.125), 1);
		gl::glMemoryBarrier(gl::GL_ALL_BARRIER_BITS);

		gl::glUseProgram(screenShaderProgram);
		gl::glBindTextureUnit(0, screenTex);
		gl::glUniform1i(gl::glGetUniformLocation(screenShaderProgram, "screen"), 0);
		gl::glBindVertexArray(VAO);
		gl::glDrawElements(gl::GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), gl::GL_UNSIGNED_INT, 0);

        window.display();
    }

	gl::glDeleteProgram(screenShaderProgram);
	gl::glDeleteProgram(computeProgram);
}
