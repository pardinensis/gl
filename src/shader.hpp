#pragma once

#include <GL/glew.h>
#include <string>

namespace shader {
	void create(const std::string& name, const std::string& filename, GLuint type);
	void remove(const std::string& name);

	GLuint use(const std::string& name);
}