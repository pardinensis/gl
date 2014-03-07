#include "camera.hpp"

#include <map>
#include <utility>
#include <iostream>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace camera {
	struct cam_info {
		glm::mat3 lookat;
		glm::vec4 frnf;
		glm::mat4 proj;
		glm::vec4 viewport;

		cam_info(glm::vec3& pos, glm::vec3& dir, glm::vec3& up,
				float fov, float ratio, float near, float far) :
				lookat(pos, dir, up), frnf(fov, ratio, near, far) {
			update_proj();
		}

		void update_proj() {
			frnf[1] = viewport[2] / viewport[3];
			proj = glm::perspective(frnf[0], frnf[1], frnf[2], frnf[3]);
		}
	};


	std::map<std::string, cam_info*> cameramap;
	std::string cur_cam_name = "";
	cam_info* cur_cam = nullptr;

	float ratio = 1;
}

void camera::create(const std::string& name, glm::vec3& pos, glm::vec3& dir, glm::vec3& up,
			float fov, float near, float far) {
	cameramap.insert(make_pair(name, new cam_info(pos, dir, up, fov, ratio, near, far)));
}

void camera::remove(const std::string& name) {
	auto it = cameramap.find(name);
	if (it != cameramap.end()) {
		if (it->second == cur_cam) {
			cur_cam = nullptr;
			cur_cam_name = "";
		}
		delete it->second;
		cameramap.erase(it);
	}
}

void camera::remove_all() {
	for (auto it = cameramap.begin(); it != cameramap.end(); ++it) {
		if (it->second == cur_cam) {
			cur_cam = nullptr;
			cur_cam_name = "";
		}
		delete it->second;
	}
	cameramap.clear();
}

void camera::use(const std::string& name) {
	auto it = cameramap.find(name);
	if (it == cameramap.end()) {
		throw std::invalid_argument("camera " + name + " not found.");
	}

	cam_info* cam = it->second;
	glViewport(cam->viewport[0], cam->viewport[1], cam->viewport[2], cam->viewport[3]);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(glm::lookAt(cam->lookat[0], cam->lookat[1], cam->lookat[2])));
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(cam->proj));

	cur_cam_name = name;
	cur_cam = cameramap[name];
}

void camera::set_viewport(int x, int y, int w, int h) {
	glm::vec4 v(x, y, w, h);
	set_viewport(v);
}

void camera::set_viewport(glm::vec4& v) {
	for (auto& p : cameramap) {
		p.second->viewport = v;
		p.second->update_proj();
	}
	use(cur_cam_name);
}

void camera::set_viewport(const std::string& name, int x, int y, int w, int h) {
	glm::vec4 v(x, y, w, h);
	set_viewport(name, v);
}

void camera::set_viewport(const std::string& name, glm::vec4& v) {
	auto it = cameramap.find(name);
	if (it == cameramap.end()) {
		throw std::invalid_argument("camera " + name + " not found.");
	}
	it->second->viewport = v;
	it->second->update_proj();
	use(cur_cam_name);
}

glm::vec4 camera::get_viewport(const std::string& name) {
	auto it = cameramap.find(name);
	if (it == cameramap.end()) {
		throw std::invalid_argument("camera " + name + " not found.");
	}
	return it->second->viewport;
}