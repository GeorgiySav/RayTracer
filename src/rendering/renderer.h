#pragma once

#include <glbinding/gl/gl.h>

namespace engine {
	extern bool initialiseRenderer(gl::GLsizei width, gl::GLsizei height);
	extern void destroyRenderer();
	extern void render();
	extern void resizeResolution(gl::GLsizei new_width, gl::GLsizei new_height);
}