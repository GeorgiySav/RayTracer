#pragma once

#include <iostream>

#include <glbinding/gl/gl.h>

namespace rendering
{

	extern gl::GLuint createVertFragProgram(const std::string& vertex_file_path, const std::string& fragment_file_path);
	extern gl::GLuint createComputeProgram(const std::string& compute_file_path);

} // namespace rendering
