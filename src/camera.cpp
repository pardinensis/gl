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
		glm::vec4 viewport;
		glm::mat4 proj;

		cam_info(glm::vec3& pos, glm::vec3& dir, glm::vec3& up,
				float fov, float ratio, float near, float far) :
				lookat(pos, dir, up), viewport(fov, ratio, near, far) {
			update_proj();
		}

		void update_proj() {
			proj = glm::perspective(viewport[0], viewport[1], viewport[2], viewport[3]);
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
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(glm::lookAt(cam->lookat[0], cam->lookat[1], cam->lookat[2])));
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(cam->proj));

	cur_cam_name = name;
	cur_cam = cameramap[name];
}

void camera::set_viewport(int w, int h) {
	glViewport(0, 0, w, h);
	ratio = (float)w / h;

	for (auto& p : cameramap) {
		p.second->viewport[1] = ratio;
		p.second->update_proj();
	}

	use(cur_cam_name);
}

