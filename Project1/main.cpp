#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "WaterSurface.h"

namespace glfwCallbacks
{
	void error(int error, const char* description)
	{
		std::cout << description;
	}

	void key(GLFWwindow* window, int key, int scancode, int action, int mods)
	{

	}
}

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		std::cout << "couldn't init glfw\n";
		return 1;
	}

	bool windowed = true;
	if (!(window = glfwCreateWindow(1920, 1080, "Terrain1", windowed ? glfwGetPrimaryMonitor() : NULL, NULL)))
	{
		std::cout << "couldn't create glfw window\n";
		glfwTerminate();
		return 2;
	}

	glfwSetErrorCallback(glfwCallbacks::error);
	glfwSetKeyCallback(window, glfwCallbacks::key);

	glfwMakeContextCurrent(window);


	if (GLEW_OK != glewInit()) //since the context is available now
	{
		std::cout << "couldn't init glew\n";
		glfwDestroyWindow(window);
		glfwTerminate();
		return 3;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glm::mat4x4 projection = glm::perspective(45.0f, width / (float)height, 0.1f, 100.0f);
	glm::mat4x4 view = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4x4 pv = projection * view;

	WaterSurface water;

	double time;
	while (!glfwWindowShouldClose(window))
	{
		time = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		water.draw(time, pv);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

