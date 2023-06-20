#pragma once

#include <iostream>
#include <map>

#include <glbinding/gl/gl.h>

namespace rendering
{

	// base class for all other shader programs. Do not use this class
	class ShaderProgram
	{
	protected:
		gl::GLuint m_program_id;

		struct Uniform
		{
			std::string name;
			gl::GLenum type;
			gl::GLuint location;
		};
		using Attribute = Uniform;

		std::map<std::string, Uniform> m_uniforms;
		std::map<std::string, Attribute> m_attributes;

	public:
		ShaderProgram() : m_program_id(0) {};
		~ShaderProgram() { gl::glDeleteProgram(m_program_id); }

		void listAttributesUniforms();

		void use() { gl::glUseProgram(m_program_id); }

		template <typename T>
		bool setUniform(const std::string& name, const T& value) {
			if (!m_uniforms.count(name)) {
				std::cout << "Doesn't contains uniform: " << name << std::endl;
				return false;
			}

			if constexpr (T == gl::GLint) {
				gl::glUniform1i(m_uniforms.at(name).location, value);
			}
			else {
				std::cout << "Not a valid data type" << std::endl;
				return false;
			}	
			return true;
		}

		bool setTexture(const std::string& name, gl::GLuint texture) {
			if (!m_uniforms.count(name)) {
				std::cout << "Doesn't contains uniform: " << name << std::endl;
				return false;
			}
			if (m_uniforms.at(name).type != gl::GL_SAMPLER_2D) {
				std::cout << name << " is not a texture" << std::endl;
				return false;
			}
			else {
				gl::glBindTextureUnit(m_uniforms.at(name).location, texture);
				return true;
			}
		}

		gl::GLuint getId() { return m_program_id; }
	};

	class VertFragProgram : public ShaderProgram
	{
	public:
		VertFragProgram(const char* vertexFP, const char* fragmentFP);
		~VertFragProgram() {}

	};

	class ComputeProgram : public ShaderProgram
	{
	public:
		ComputeProgram(const char* filepath);
		~ComputeProgram() {}

		void dispatch(gl::GLuint x, gl::GLuint y, gl::GLuint z, gl::MemoryBarrierMask m);
	};

} // namespace rendering
