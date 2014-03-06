#pragma once

#include <glm/glm.hpp>
#include <string>

namespace camera {
	void create(const std::string& name, glm::vec3& pos, glm::vec3& dir, glm::vec3& up,
		float fov, float near, float far);
	void remove(const std::string& name);
	void remove_all();

	void use(const std::string& name);

	void set_viewport(int w, int h);
}