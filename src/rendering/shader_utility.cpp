#include "shader_utility.h"

#include <fstream>

namespace rendering
{
	std::string loadShader(const std::string& file_path) {
        constexpr auto read_size = std::size_t(4096);
        std::ifstream stream;
		stream.open(file_path.c_str());
        stream.exceptions(std::ios_base::badbit);
		
        if (stream.fail()) {
            throw std::ios_base::failure("Shader file does not exist: " + file_path);
        }

        std::string source = "";
        auto buf = std::string(read_size, '\0');
        while (stream.read(&buf[0], read_size)) {
            source.append(buf, 0, stream.gcount());
        }
        source.append(buf, 0, stream.gcount());
        return source;
	}

	gl::GLuint createVertFragProgram(const std::string& vertex_file_path, const std::string& fragment_file_path) {
        std::string vertex_source = loadShader(vertex_file_path);
        std::string fragment_source = loadShader(fragment_file_path);
		const char* vert_source = vertex_source.c_str();
		const char* frag_source = fragment_source.c_str();

		gl::GLuint vertex_shader = gl::glCreateShader(gl::GL_VERTEX_SHADER);
		gl::glShaderSource(vertex_shader, 1, &vert_source, NULL);
		gl::glCompileShader(vertex_shader);
		gl::GLuint fragment_shader = gl::glCreateShader(gl::GL_FRAGMENT_SHADER);
		gl::glShaderSource(fragment_shader, 1, &frag_source, NULL);
		gl::glCompileShader(fragment_shader);

		gl::GLuint shader_program = gl::glCreateProgram();
		gl::glAttachShader(shader_program, vertex_shader);
		gl::glAttachShader(shader_program, fragment_shader);
		gl::glLinkProgram(shader_program);

		gl::glDeleteShader(vertex_shader);
		gl::glDeleteShader(fragment_shader);

		return shader_program;
	}

	gl::GLuint createComputeProgram(const std::string& compute_file_path) {
		std::string compute_source = loadShader(compute_file_path);
		const char* comp_source = compute_source.c_str();

		gl::GLuint compute_shader = gl::glCreateShader(gl::GL_COMPUTE_SHADER);
		gl::glShaderSource(compute_shader, 1, &comp_source, NULL);
		gl::glCompileShader(compute_shader);

		gl::GLuint compute_program = gl::glCreateProgram();
		gl::glAttachShader(compute_program, compute_shader);
		gl::glLinkProgram(compute_program);

		gl::glDeleteShader(compute_shader);

		return compute_program;
	}
}