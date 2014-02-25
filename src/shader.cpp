#include "shader.hpp"

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <stdexcept>

namespace shader {
	struct program {
		GLuint id;
		bool linked;
		std::vector<GLuint> shaders;

		program() : id(glCreateProgram()), linked(false) {};

		void attach(GLuint shader) {
			shaders.push_back(shader);
			glAttachShader(id, shader);
		}
	};

	std::map<std::string, program> shadermap;
}

void print_log(GLuint shader) {
	GLint max_len = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_len);
	
	std::vector<char> info_log(max_len);
	glGetShaderInfoLog(shader, max_len, &max_len, &info_log[0]);

	std::string info_log_str(info_log.begin(), info_log.end());
	std::cerr << info_log_str << std::endl;
}

void insert(GLuint shader, const std::string& name) {
	using namespace shader;
	auto it = shadermap.find(name);
	if (it == shadermap.end()) {
		program p;
		p.attach(shader);
		shadermap.insert(std::make_pair(name, p));
		glDeleteShader(shader);
	} else {
		it->second.attach(shader);
	}
 }

void shader::create(const std::string& name, const std::string& filename, GLuint type) {
	// get the shader code in a char[] format
	std::ifstream ifs ((filename.c_str()));
	std::string str ((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	const GLchar* source = static_cast<const GLchar*>(str.c_str());

	//create shader object and compile code
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);
	
	//error handling
	GLint compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE) {
		print_log(shader);
		exit(EXIT_FAILURE);
	}

	insert(shader, name);
}

void shader::remove(const std::string& name) {
	auto it = shadermap.find(name);
	if (it != shadermap.end()) {
		program& p = it->second;
		glDeleteProgram(p.id);
		for (GLuint shader : p.shaders) {
			glDetachShader(p.id, shader);
			glDeleteShader(shader);
		}
		shadermap.erase(it);
	}
}

void shader::remove_all() {
	while (!shadermap.empty()) {
		auto& p = shadermap.begin()->second;
		glDeleteProgram(p.id);
		for (GLuint shader : p.shaders) {
			glDetachShader(p.id, shader);
			glDeleteShader(shader);
		}
		shadermap.erase(shadermap.begin());
	}
}

GLuint shader::use(const std::string& name) {
	using namespace shader;
	auto it = shadermap.find(name);
	if (it == shadermap.end()) {
		std::invalid_argument ex("shader program " + name + " not found.");
		throw ex;
	}

	program& p = it->second;
	if (!p.linked) {
		glLinkProgram(p.id);
		GLint is_linked = 0;
		glGetProgramiv(p.id, GL_LINK_STATUS, (GLint *)&is_linked);
		if (is_linked == GL_FALSE) {
			GLint max_len = 0;
			glGetProgramiv(p.id, GL_INFO_LOG_LENGTH, &max_len);
			std::vector<char> info_log(max_len);
			glGetProgramInfoLog(p.id, max_len, &max_len, &info_log[0]);
			std::string info_log_str(info_log.begin(), info_log.end());
			std::cerr << info_log_str << std::endl;
			exit(EXIT_FAILURE);
		}

		p.linked = true;
		for (GLuint shader : p.shaders) {
			glDetachShader(p.id, shader);
		}
	}

	glUseProgram(p.id);

	return p.id;
}