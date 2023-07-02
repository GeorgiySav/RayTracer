#include "renderer.h"

#include "shader.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace engine {

	Renderer::Renderer() {
		
		// create vertex arrays and buffers
		gl::glCreateVertexArrays(1, &m_screen_vao);
		gl::glCreateBuffers(1, &m_screen_vbo);
		gl::glCreateBuffers(1, &m_screen_ebo);
		// initialise the buffers
		gl::glNamedBufferData(m_screen_vbo, sizeof(m_screen_vertices), m_screen_vertices, gl::GL_STATIC_DRAW);
		gl::glNamedBufferData(m_screen_ebo, sizeof(m_screen_indices), m_screen_indices, gl::GL_STATIC_DRAW);
		// determine what data the buffers hold
		gl::glEnableVertexArrayAttrib(m_screen_vao, 0);
		gl::glVertexArrayAttribBinding(m_screen_vao, 0, 0);
		gl::glVertexArrayAttribFormat(m_screen_vao, 0, 3, gl::GL_FLOAT, gl::GL_FALSE, 0);

		gl::glEnableVertexArrayAttrib(m_screen_vao, 1);
		gl::glVertexArrayAttribBinding(m_screen_vao, 1, 0);
		gl::glVertexArrayAttribFormat(m_screen_vao, 1, 2, gl::GL_FLOAT, gl::GL_FALSE, 3 * sizeof(gl::GLfloat));

		gl::glVertexArrayVertexBuffer(m_screen_vao, 0, m_screen_vbo, 0, 5 * sizeof(gl::GLfloat));
		gl::glVertexArrayElementBuffer(m_screen_vao, m_screen_ebo);

		// create shaders
		m_render_screen_program.loadfromFile("../../../src/shaders/screen_vert.vert", "../../../src/shaders/screen_frag.frag");
		m_ray_tracing_program.loadFromFile("../../../src/shaders/ray_tracer.comp");
	}

	Renderer::~Renderer() {
		gl::glDeleteTextures(1, &m_screen_texture);
		gl::glDeleteVertexArrays(1, &m_screen_vao);
		gl::glDeleteBuffers(1, &m_screen_vbo);
		gl::glDeleteBuffers(1, &m_screen_ebo);
	}

	void Renderer::create(gl::GLsizei width, gl::GLsizei height) {
		m_screen_width = width, m_screen_height = height;
		// create texture
		gl::glGenTextures(1, &m_screen_texture);
		gl::glActiveTexture(gl::GL_TEXTURE0);
		// define characteristics
		gl::glBindTexture(gl::GL_TEXTURE_2D, m_screen_texture);
		gl::glTextureParameteri(m_screen_texture, gl::GL_TEXTURE_MIN_FILTER, gl::GL_NEAREST);
		gl::glTextureParameteri(m_screen_texture, gl::GL_TEXTURE_MAG_FILTER, gl::GL_NEAREST);
		gl::glTextureParameteri(m_screen_texture, gl::GL_TEXTURE_WRAP_S, gl::GL_LINEAR);
		gl::glTextureParameteri(m_screen_texture, gl::GL_TEXTURE_WRAP_T, gl::GL_LINEAR);
		gl::glTexImage2D(gl::GL_TEXTURE_2D, 0, gl::GL_RGBA32F, width, height, 0, gl::GL_RGBA, gl::GL_FLOAT, NULL);
		gl::glBindImageTexture(0, m_screen_texture, 0, gl::GL_FALSE, 0, gl::GL_WRITE_ONLY, gl::GL_RGBA32F); // bind to the compute shader
		// unbind
		gl::glBindTexture(gl::GL_TEXTURE_2D, 0);
	}

	void Renderer::resize(gl::GLsizei new_width, gl::GLsizei new_height) {
		m_screen_width = new_width, m_screen_height = new_height;
		//camera.setHwAspect((float)m_screen_height / (float)m_screen_width);
		gl::glBindTexture(gl::GL_TEXTURE_2D, m_screen_texture);
		gl::glTexImage2D(gl::GL_TEXTURE_2D, 0, gl::GL_RGBA32F, new_width, new_height, 0, gl::GL_RGBA, gl::GL_FLOAT, NULL);
		gl::glBindTexture(gl::GL_TEXTURE_2D, 0);
	}

	void Renderer::render(const std::unique_ptr<Scene>& scene) {
		const Camera& scene_camera = scene->getCamera();
		this->prepareFrame(scene_camera);
		this->renderFrame();
	}

	void Renderer::prepareFrame(const Camera& camera) {
		// perform the ray tracing a clear the screen
		m_ray_tracing_program.use();
		gl::glUniformMatrix4fv(gl::glGetUniformLocation(m_ray_tracing_program.getId(), "cam_matrix"), 1, gl::GL_FALSE, camera.getCameraMatrixPointer());
		gl::glUniform3fv(gl::glGetUniformLocation(m_ray_tracing_program.getId(), "cam_pos"), 1, camera.getPositionPointer());
		gl::glUniform2fv(gl::glGetUniformLocation(m_ray_tracing_program.getId(), "plane_dims"), 1, camera.getPlaneDimsPointer());
		gl::glUniform1f(gl::glGetUniformLocation(m_ray_tracing_program.getId(), "near_plane"), camera.getNear());
		gl::glUniform1f(gl::glGetUniformLocation(m_ray_tracing_program.getId(), "far_plane"), camera.getFar());
		m_ray_tracing_program.dispatch(std::ceil(m_screen_width * 0.125), std::ceil(m_screen_height * 0.125), 1, gl::GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		gl::glClear(gl::GL_COLOR_BUFFER_BIT);
	}

	void Renderer::renderFrame() {
		// render the texture to the screen
		m_render_screen_program.use();
		m_render_screen_program.setTexture("screen", m_screen_texture);
		gl::glBindVertexArray(m_screen_vao);
		gl::glDrawElements(gl::GL_TRIANGLES, sizeof(m_screen_indices) / sizeof(m_screen_indices[0]), gl::GL_UNSIGNED_INT, 0);
	}

}

