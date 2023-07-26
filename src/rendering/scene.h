#pragma once

#include <SFML/Window.hpp>
#include <glbinding/gl/gl.h>

#include "objects.h"
#include "camera.h"

namespace engine {
	/// <summary>
	/// Stores all the vertices, mesh info of all objects/entities in a scene
	/// </summary>
	class Scene {
	public:
		Scene();
		~Scene();

		void processKeyHolds();
		void processKeyPress(sf::Event& e);
		void processMouseMovement(int middle_x, int middle_y);

		void update();

		void addCircle();

		void setHwAspect(float ratio) { m_camera.setHwAspect(ratio); }
		void moveCamera(const glm::vec3& offset) { m_camera.move(offset); }
		const Camera& getCamera() const { return m_camera; }

		const gl::GLsizei getCircleBufferSize() const { return m_circle_ssbo.size; }
		const gl::GLsizei getPlaneBufferSize() const { return m_plane_ssbo.size; }
	private:
		Camera m_camera;

		struct SSBO {
			gl::GLuint id;
			gl::GLsizei size;
			gl::GLsizei max_size;
			template <typename T>
			void create(gl::GLsizei max_size, gl::GLuint binding) {
				this->max_size = max_size;
				this->size = 0;
				gl::glGenBuffers(1, &this->id);
				gl::glBindBuffer(gl::GL_SHADER_STORAGE_BUFFER, this->id);
				gl::glBufferData(gl::GL_SHADER_STORAGE_BUFFER, sizeof(T) * this->max_size, NULL, gl::GL_DYNAMIC_READ);
				gl::glBindBufferBase(gl::GL_SHADER_STORAGE_BUFFER, binding, this->id);
				gl::glBindBuffer(gl::GL_SHADER_STORAGE_BUFFER, 0); // unbind	
			}
			~SSBO() { gl::glDeleteBuffers(1, &id); }
			template <typename T>
			void add(T& data) {
				if (size >= max_size)
					return;
				gl::glBindBuffer(gl::GL_SHADER_STORAGE_BUFFER, id);
				//void* raw_data = gl::glMapBufferRange(gl::GL_SHADER_STORAGE_BUFFER, size, (size + 1) * item_size, gl::GL_MAP_WRITE_BIT | gl::GL_MAP_INVALIDATE_BUFFER_BIT);
				//T* ssbo_data = static_cast<T*>(raw_data);
				//ssbo_data[size] = data;
				//gl::glUnmapBuffer(gl::GL_SHADER_STORAGE_BUFFER);
				gl::glBufferSubData(gl::GL_SHADER_STORAGE_BUFFER, size * sizeof(T), sizeof(T), &data);
				gl::glBindBuffer(gl::GL_SHADER_STORAGE_BUFFER, 0);
				size++;
			}
		};
		SSBO m_circle_ssbo;
		SSBO m_plane_ssbo;
	};
}