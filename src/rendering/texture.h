#pragma once

#include <glbinding/gl/gl.h>

namespace rendering
{
	class Texture2D
	{
	private:
		gl::GLuint m_id;
		gl::GLsizei m_width, m_height;
		gl::GLenum m_format;

		static gl::GLint m_unit;
	public:
		Texture2D() : m_id(0), m_width(0), m_height(0), m_format() { m_unit++; }
		~Texture2D() { gl::glDeleteTextures(1, &m_id); }

		void create(gl::GLsizei width,
			gl::GLsizei height,
			gl::GLenum format,
			gl::GLenum min_filter,
			gl::GLenum mag_filter,
			gl::GLenum wrap_s,
			gl::GLenum wrap_t,
			gl::GLenum access) {
			m_width = width;
			m_height = height;
			m_format = format;
			gl::glCreateTextures(gl::GL_TEXTURE_2D, 1, &m_id);
			gl::glTextureParameteri(m_id, gl::GL_TEXTURE_MIN_FILTER, min_filter);
			gl::glTextureParameteri(m_id, gl::GL_TEXTURE_MAG_FILTER, mag_filter);
			gl::glTextureParameteri(m_id, gl::GL_TEXTURE_WRAP_S, wrap_s);
			gl::glTextureParameteri(m_id, gl::GL_TEXTURE_WRAP_T, wrap_t);
			gl::glTextureStorage2D(m_id, 1, format, width, height);
			gl::glBindImageTexture(m_unit, m_id, 0, gl::GL_FALSE, 0, access, format);
		}
	
		gl::GLuint getId() { return m_id; }
	};
	gl::GLint Texture2D::m_unit = -1;
}