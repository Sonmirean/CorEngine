
#define GLFW_INCLUDE_VULKAN

#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <thread>
#include "DimensionUtils.h"
#include "WindowManager.h"
#include "AppStructure.h"




Window::Window(WindowProperties* props) :
		title(props->title), 
		width(props->width), 
		height(props->height), 
		v_sync(props->v_sync),
		share(props->share),
		fov(props->fov),
		proj_mat(props->proj_mat),
		x_pos(props->x_pos),
		y_pos(props->y_pos),
		z_near(props->z_near),
		z_far(props->z_far)
{

	handle = glfwCreateWindow(width, height, title, /*props->monitor ? props->monitor :*/ nullptr, /*props->share ? props->share :*/ nullptr);
	if (!handle)
	{
		throw std::runtime_error("Failed to create window.");
		delete this;
		return;
	}

	GLFWwindow* prev = glfwGetCurrentContext();
	glfwMakeContextCurrent(handle);

	if (v_sync) {
		glfwSwapInterval(1);
	}

	glfwMakeContextCurrent(prev);

	refreshProjMat();
	setPos(x_pos, y_pos);

	glfwShowWindow(handle);

	

	AppStructure::ExternalClassAccess::addWindow(this);
}

Window::~Window()
{
	glfwDestroyWindow(handle);
}

/*
bool Window::operator ==(Window win)
{
	//cyka
}
*/

void Window::refreshProjMat()
{
	proj_mat = glm::perspective(fov, static_cast<float>(width / height), z_near, z_far);
}

void Window::centralize()
{
	int xPos = (glfwGetVideoMode(glfwGetPrimaryMonitor())->width / 2) - (width / 2);
	int yPos = (glfwGetVideoMode(glfwGetPrimaryMonitor())->height / 2) - (height / 2);
	glfwSetWindowPos(handle, xPos, yPos);
}

void Window::update()
{
	refreshProjMat();
	swapBuffers();
	pollEvents();
}

void Window::swapBuffers()
{
	if (!shouldClose())
		glfwSwapBuffers(handle);
}

void Window::pollEvents()
{
	if (!shouldClose())
	{
		GLFWwindow* prev = glfwGetCurrentContext();
		glfwMakeContextCurrent(handle);
		glfwPollEvents();
		glfwMakeContextCurrent(prev);
	}
	
}

void Window::setLimits(unsigned int min_w, unsigned int min_h, unsigned int max_w, unsigned int max_h)
{
	if ((max_w > this->width > min_w) && (max_h > this->height > min_h))
	{
		glfwSetWindowSizeLimits(handle, min_w, min_h, max_w, max_h);
	}
	else
	{
		setSize(max_w, max_h);
		glfwSetWindowSizeLimits(handle, min_w, min_h, max_w, max_h);
	}
}

const char* Window::getTitle() const
{
	return title;
}

unsigned int Window::getPos(Dim dim) const
{
	return (dim == X) ? x_pos : y_pos;
}

unsigned int Window::getSize(Dim dim) const
{
	return (dim == X) ? width : height;
}

bool Window::isVSync() const
{
	return v_sync;
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(handle);
}

GLFWwindow* Window::getHandle() const
{
	return handle;
}

GLFWwindow* Window::getShare() const
{
	return share;
}

GLFWmonitor* Window::getMonitor() const
{
	return glfwGetWindowMonitor(handle);
}

float Window::getNearClip() const
{
	return z_near;
}

float Window::getFarClip() const
{
	return z_far;
}

float Window::getFieldOfView() const
{
	return fov;
}

glm::mat4x4 Window::getProjMat() const
{
	return proj_mat;
}

void Window::setContextCurrent()
{
	glfwMakeContextCurrent(handle);
}

void Window::setSize(unsigned int w, unsigned int h)
{
	glfwSetWindowSize(handle, w, h);
	this->width = w;
	this->height = h;
}

void Window::setPos(unsigned int x, unsigned int y)
{
	glfwSetWindowPos(handle, x, y);
	this->x_pos = x;
	this->y_pos = y;
}

void Window::setVSync(bool b)
{
	v_sync = b;
	GLFWwindow* prev = glfwGetCurrentContext();
	glfwMakeContextCurrent(handle);
	b ? glfwSwapInterval(1) : glfwSwapInterval(0);
	glfwMakeContextCurrent(prev);
}

void Window::setTitle(const char* title)
{
	this->title = title;
	GLFWwindow* prev = glfwGetCurrentContext();
	glfwMakeContextCurrent(handle);
	glfwSetWindowTitle(handle, this->title);
	glfwMakeContextCurrent(prev);
}

void Window::setShouldClose(bool b)
{
	GLFWwindow* prev = glfwGetCurrentContext();
	glfwMakeContextCurrent(handle);
	glfwSetWindowShouldClose(handle, b);
	glfwMakeContextCurrent(prev);
}

void Window::setNearClip(float z_near)
{
	this->z_near = z_near;
	refreshProjMat();
}

void Window::setFarClip(float z_far)
{
	this->z_far = z_far;
	refreshProjMat();
}

void Window::setFieldOfView(float fov)
{
	this->fov = fov;
	refreshProjMat();
}

