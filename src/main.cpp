#include "common.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"

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

	//handle user inputs
	glfwSetKeyCallback(window, key_callback);

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

	//run main loop
	glClearColor(0, 0, 0, 0);
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(window)) {
		//clear
		glClear(GL_COLOR_BUFFER_BIT);
		
		//set viewport and camera matrices
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		float ratio = width / static_cast<float>(height);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

		//draw stuff
		display();
		glfwSwapBuffers(window);

		//handle events
		glfwPollEvents();

		count_fps();
	}

	//clean up
	shader::remove_all();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}