#include "shader.h"

#include <fstream>

namespace engine
{
	std::string readFromFile(const char* filepath)
	{
		std::ifstream in(filepath, std::ios::binary);
		if (in)
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return(contents);
		}
		throw(errno);
	}

	void ShaderProgram::listAttributesUniforms()
	{
		gl::GLint i;
		gl::GLint count;

		gl::GLint size; // size of the variable
		gl::GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const gl::GLsizei bufSize = 16; // maximum name length
		gl::GLchar name[bufSize]; // variable name in GLSL
		gl::GLsizei length; // name length

		gl::glGetProgramiv(m_program_id, gl::GL_ACTIVE_ATTRIBUTES, &count);
		std::cout << "Active Attributes: " << count << std::endl;
		for (i = 0; i < count; i++) {
			gl::glGetActiveAttrib(m_program_id, (gl::GLuint)i, bufSize, &length, &size, &type, name);
			printf("Attribute #%d Type: %u Name: %s\n", i, type, name);
			Attribute temp = { name, type, i };
			m_attributes.insert({ name, temp });
		}

		gl::glGetProgramiv(m_program_id, gl::GL_ACTIVE_UNIFORMS, &count);
		std::cout << "Active Uniforms: " << count << std::endl;
		for (i = 0; i < count; i++) {
			gl::glGetActiveUniform(m_program_id, (gl::GLuint)i, bufSize, &length, &size, &type, name);
			printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
			Uniform temp = {name, type, i};
			m_uniforms.insert({ name, temp });
		}

		std::cout << "\n\n";
	}


	void VertFragProgram::loadfromFile(const char* vertexFP, const char* fragmentVP)
	{
		std::string vS = readFromFile(vertexFP);
		std::string fS = readFromFile(fragmentVP);

		const char* vertexSource = vS.c_str();
		const char* fragmentSource = fS.c_str();

		gl::GLuint vertexShader = gl::glCreateShader(gl::GL_VERTEX_SHADER);
		gl::glShaderSource(vertexShader, 1, &vertexSource, NULL);
		gl::glCompileShader(vertexShader);

		gl::GLuint fragmentShader = gl::glCreateShader(gl::GL_FRAGMENT_SHADER);
		gl::glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		gl::glCompileShader(fragmentShader);

		m_program_id = gl::glCreateProgram();
		gl::glAttachShader(m_program_id, vertexShader);
		gl::glAttachShader(m_program_id, fragmentShader);
		gl::glLinkProgram(m_program_id);

		gl::glDeleteShader(vertexShader);
		gl::glDeleteShader(fragmentShader);

		listAttributesUniforms();
	}

	void ComputeProgram::loadFromFile(const char* filepath)
	{
		std::string cS = readFromFile(filepath);

		const char* computeSource = cS.c_str();

		gl::GLuint computeShader = gl::glCreateShader(gl::GL_COMPUTE_SHADER);
		gl::glShaderSource(computeShader, 1, &computeSource, NULL);
		gl::glCompileShader(computeShader);

		m_program_id = gl::glCreateProgram();
		gl::glAttachShader(m_program_id, computeShader);
		gl::glLinkProgram(m_program_id);

		gl::glDeleteProgram(computeShader);

		listAttributesUniforms();
	}

	void ComputeProgram::dispatch(gl::GLuint x, gl::GLuint y, gl::GLuint z, gl::MemoryBarrierMask m)
	{
		gl::glDispatchCompute(x, y, z);
		gl::glMemoryBarrier(m);
	}
}