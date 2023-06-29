#include "renderer.h"

#include "shader.h"

namespace engine {

	// define variables used in rendering
	gl::GLfloat screen_vertices[] = {
		-1.0f, -1.0f , 0.0f, 0.0f, 0.0f,
		-1.0f,  1.0f , 0.0f, 0.0f, 1.0f,
		 1.0f,  1.0f , 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f , 0.0f, 1.0f, 0.0f,
	};
	gl::GLuint screen_indices[] = {
		0, 2, 1,
		0, 3, 2
	};

	gl::GLuint screen_vao, screen_vbo, screen_ebo;
	gl::GLuint screen_texture;
	gl::GLuint screen_width, screen_height;
	VertFragProgram render_screen_program;
	ComputeProgram ray_tracing_program;

	// define functions exclusive to this file to use in rendering
	void prepareFrame();
	void renderFrame();

	bool initialiseRenderer(gl::GLsizei width, gl::GLsizei height) {
		screen_width = width, screen_height = height;
		// prepare rendering to a screen
		// create vertex arrays and buffers
		gl::glCreateVertexArrays(1, &screen_vao);
		gl::glCreateBuffers(1, &screen_vbo);
		gl::glCreateBuffers(1, &screen_ebo);
		// initialise the buffers
		gl::glNamedBufferData(screen_vbo, sizeof(screen_vertices), screen_vertices, gl::GL_STATIC_DRAW);
		gl::glNamedBufferData(screen_ebo, sizeof(screen_indices), screen_indices, gl::GL_STATIC_DRAW);
		// determine what data the buffers hold
		gl::glEnableVertexArrayAttrib(screen_vao, 0);
		gl::glVertexArrayAttribBinding(screen_vao, 0, 0);
		gl::glVertexArrayAttribFormat(screen_vao, 0, 3, gl::GL_FLOAT, gl::GL_FALSE, 0);

		gl::glEnableVertexArrayAttrib(screen_vao, 1);
		gl::glVertexArrayAttribBinding(screen_vao, 1, 0);
		gl::glVertexArrayAttribFormat(screen_vao, 1, 2, gl::GL_FLOAT, gl::GL_FALSE, 3 * sizeof(gl::GLfloat));
		
		gl::glVertexArrayVertexBuffer(screen_vao, 0, screen_vbo, 0, 5 * sizeof(gl::GLfloat));
		gl::glVertexArrayElementBuffer(screen_vao, screen_ebo);

		// create texture
		gl::glGenTextures(1, &screen_texture);
		gl::glActiveTexture(gl::GL_TEXTURE0);
		// define characteristics
		gl::glBindTexture(gl::GL_TEXTURE_2D, screen_texture);
		gl::glTextureParameteri(screen_texture, gl::GL_TEXTURE_MIN_FILTER, gl::GL_NEAREST);
		gl::glTextureParameteri(screen_texture, gl::GL_TEXTURE_MAG_FILTER, gl::GL_NEAREST);
		gl::glTextureParameteri(screen_texture, gl::GL_TEXTURE_WRAP_S, gl::GL_LINEAR);
		gl::glTextureParameteri(screen_texture, gl::GL_TEXTURE_WRAP_T, gl::GL_LINEAR);
		gl::glTexImage2D(gl::GL_TEXTURE_2D, 0, gl::GL_RGBA32F, width, height, 0, gl::GL_RGBA, gl::GL_FLOAT, NULL);
		gl::glBindImageTexture(0, screen_texture, 0, gl::GL_FALSE, 0, gl::GL_WRITE_ONLY, gl::GL_RGBA32F); // bind to the compute shader
		// unbind
		gl::glBindTexture(gl::GL_TEXTURE_2D, 0);

		// create shaders
		render_screen_program.loadfromFile("../../../src/shaders/screen_vert.vert", "../../../src/shaders/screen_frag.frag");
		ray_tracing_program.loadFromFile("../../../src/shaders/ray_tracer.comp");

		return true;
	}

	void destroyRenderer() {
		gl::glDeleteTextures(1, &screen_texture);
		gl::glDeleteVertexArrays(1, &screen_vao);
		gl::glDeleteBuffers(1, &screen_vbo);
		gl::glDeleteBuffers(1, &screen_ebo);
	}

	void resizeResolution(gl::GLsizei new_width, gl::GLsizei new_height) {
		screen_width = new_width, screen_height = new_height;
		gl::glBindTexture(gl::GL_TEXTURE_2D, screen_texture);
		gl::glTexImage2D(gl::GL_TEXTURE_2D, 0, gl::GL_RGBA32F, new_width, new_height, 0, gl::GL_RGBA, gl::GL_FLOAT, NULL);
		gl::glBindTexture(gl::GL_TEXTURE_2D, 0);
	}

	void render() {
		prepareFrame();
		renderFrame();
	}


	void prepareFrame() {
		gl::glClear(gl::GL_COLOR_BUFFER_BIT);

		// perform the ray tracing a clear the screen
		ray_tracing_program.use();
		ray_tracing_program.dispatch(std::ceil(screen_width * 0.125), std::ceil(screen_height * 0.125), 1, gl::GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		
	}

	void renderFrame() {
		// render the texture to the screen
		render_screen_program.use();
		render_screen_program.setTexture("screen", screen_texture);
		gl::glBindVertexArray(screen_vao);
		gl::glDrawElements(gl::GL_TRIANGLES, sizeof(screen_indices) / sizeof(screen_indices[0]), gl::GL_UNSIGNED_INT, 0);
	}
}

