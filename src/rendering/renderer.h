#pragma once

#include <glbinding/gl/gl.h>

#include "shader.h"
#include "camera.h"
#include "scene.h"

namespace engine {

	class Renderer {
	public:
		Renderer();
		~Renderer();

		void create(gl::GLsizei width, gl::GLsizei height);
		void resize(gl::GLsizei new_width, gl::GLsizei new_height);

		void render(const std::unique_ptr<Scene>& scene);

	private:
		gl::GLuint m_screen_vao, m_screen_vbo, m_screen_ebo;
		gl::GLuint m_screen_texture;
		gl::GLuint m_screen_width, m_screen_height;
		VertFragProgram m_render_screen_program;
		ComputeProgram m_ray_tracing_program;

		gl::GLfloat m_screen_vertices[20] = {
			-1.0f, -1.0f , 0.0f, 0.0f, 0.0f,
			-1.0f,  1.0f , 0.0f, 0.0f, 1.0f,
			 1.0f,  1.0f , 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f , 0.0f, 1.0f, 0.0f,
		};
		gl::GLuint m_screen_indices[6] = {
			0, 2, 1,
			0, 3, 2
		};

		void prepareFrame(const std::unique_ptr<Scene>& scene);
		void renderFrame();
	};
}