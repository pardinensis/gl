#pragma once

#include <glm/glm.hpp>
#include <string>

namespace camera {
	void create(const std::string& name, glm::vec3& pos, glm::vec3& dir, glm::vec3& up, float fov, float near, float far);
	void remove(const std::string& name);
	void remove_all();

	void use(const std::string& name);

	void set_viewport(int x, int y, int w, int h);
	void set_viewport(glm::vec4& vp);
	void set_viewport(const std::string& name, int x, int y, int w, int h);
	void set_viewport(const std::string& name, glm::vec4& vp);
	glm::vec4 get_viewport(const std::string& name);
}