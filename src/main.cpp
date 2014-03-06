#include "common.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "camera.hpp"

void display() {
	GLuint id = shader::use("shader");
	GLint loc = glGetUniformLocation(id, "time");
	glUniform1f(loc, glfwGetTime());

	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-0.5f, -0.5f, 0.f);
	glVertex3f(0.5f, -0.5f, 0.f);
	glVertex3f(-0.5f, 0.5f, 0.f);
	glVertex3f(0.5f, 0.5f, 0.f);
	glEnd();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void count_fps() {
	static int fps_cntr = 0;
	static float last_time = 0;
	float t = glfwGetTime();
	if (((int)last_time) < ((int)t)) {
		std::cout << fps_cntr << std::endl;
		fps_cntr = 0;
	}
	++fps_cntr;
	last_time = t;
}

int main(void) {
	//init GLFW
	if (!glfwInit()) {
		cerr << "cannot initialize glfw" << endl;
		return 1;
	}
	
	//print all errors to cerr
	glfwSetErrorCallback([](int error, const char* desc) {
		cerr << "ERROR(" << error << "): " << desc << endl;
	});

	//create window
	GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL", nullptr, nullptr);
	if (window == nullptr) {
		cerr << "cannot create window" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	//init GLEW
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		cerr << "error: " << glewGetErrorString(err) << endl;
		exit(EXIT_FAILURE);
	}
	std::clog << "OpenGL Version: " << glGetString(GL_VERSION) << endl;

	//load shaders
	shader::create("shader", "shader/vs.glsl", GL_VERTEX_SHADER);
	shader::create("shader", "shader/fs.glsl", GL_FRAGMENT_SHADER);

	//load camera
	glm::vec3 pos(1, 1, 1);
	glm::vec3 dir(0, 0, 0);
	glm::vec3 up(0, 1, 0);
	camera::create("cam", pos, dir, up, 90, 0.01, 100);
	camera::use("cam");

	//initially set the viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	camera::set_viewport(width, height);

	//handle user inputs
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int w, int h) {
		camera::set_viewport(w, h);
	});
	
	//run main loop
	glClearColor(0, 0, 0, 0);
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(window)) {
		//clear
		glClear(GL_COLOR_BUFFER_BIT);
		
		//draw stuff
		display();
		glfwSwapBuffers(window);

		//handle events
		glfwPollEvents();

		//count_fps();
	}

	//clean up
	shader::remove_all();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}