
#include <iostream>
#include <thread>

#include "core_manager.hpp"
#include "window_manager.hpp"
#include "matrix.hpp"

#ifdef CORENGINE_USE_PLATFORM_WIN32
void CorE::Windowing::Window::initWin32Surface(HINSTANCE hinstance, HWND hwnd)
{
	const wchar_t* new_title = nullptr;

	std::string old_title_str;
	old_title_str = title;

	std::string new_title_str;
	for (size_t i = 0; i < old_title_str.length(); i++)
	{
		new_title_str += static_cast<wchar_t>(old_title_str[i]);
	}

	std::cout << new_title;


	VkWin32SurfaceCreateInfoKHR info{};
	info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	info.hinstance = hinstance;
	info.hwnd = hwnd;

	ensureVkSuccess(vkCreateWin32SurfaceKHR(CorE::Application::instance, &info, nullptr, &vk_surface));
}
#endif 

CorE::Windowing::Window::Window(CorE::Windowing::WindowProperties* props) :
		title(props->title), 
		width(props->width), 
		height(props->height), 
		v_sync(props->v_sync),
		//share(props->share),
		fov(props->fov),
		proj_mat(props->proj_mat),
		x_pos(props->x_pos),
		y_pos(props->y_pos),
		z_near(props->z_near),
		z_far(props->z_far)
{
	if (CorE::Application::instance == nullptr)
	{
		throw std::runtime_error("Cannot create window before initializing Vulkan.");
	}

	/*
	handle = glfwCreateWindow(width, height, title, props->monitor ? props->monitor : nullptr, props->share ? props->share : nullptr);
	if (!handle)
	{
		throw std::runtime_error("Failed to create window.");
	}

	GLFWwindow* prev = glfwGetCurrentContext();
	glfwMakeContextCurrent(handle);

	if (v_sync) {
		glfwSwapInterval(1);
	}
	glfwMakeContextCurrent(prev);
	refreshProjMat();
	setPos(x_pos, y_pos);
	Application::app_windows.push_back(this);
	glfwShowWindow(handle);
	

	initVkSurface(handle, &vk_surface);

	*/
}

CorE::Windowing::Window::~Window()
{
	vkDestroySurfaceKHR(CorE::Application::instance, vk_surface, nullptr);
	//glfwDestroyWindow(handle);
}

// THIS METHOD IS NOT WORKING, Mat4x4::projection() HAS EMPTY DEFINITION!!!
// TODO
void CorE::Windowing::Window::refreshProjMat()
{
	proj_mat = CorE::math::Mat4x4::projection(fov, static_cast<float>(width / height), z_near, z_far);
}

void CorE::Windowing::Window::centralize()
{
	//int xPos = (glfwGetVideoMode(glfwGetPrimaryMonitor())->width / 2) - (width / 2);
	//int yPos = (glfwGetVideoMode(glfwGetPrimaryMonitor())->height / 2) - (height / 2);
	//glfwSetWindowPos(handle, xPos, yPos);
}

void CorE::Windowing::Window::update()
{
	refreshProjMat();
	swapBuffers();
	//pollEvents();
}

void CorE::Windowing::Window::swapBuffers()
{
	//if (!shouldClose())
		//glfwSwapBuffers(handle);
}

/*
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
*/

const char* CorE::Windowing::Window::getTitle() const
{
	return title;
}

unsigned int CorE::Windowing::Window::getPos(char dim) const
{
	return (dim == 'x') ? x_pos : y_pos;
}

unsigned int CorE::Windowing::Window::getSize(char dim) const
{
	return (dim == 'x') ? width : height;
}

bool CorE::Windowing::Window::isVSync() const
{
	return v_sync;
}

/*
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
*/

float CorE::Windowing::Window::getNearClip() const
{
	return z_near;
}

float CorE::Windowing::Window::getFarClip() const
{
	return z_far;
}

float CorE::Windowing::Window::getFieldOfView() const
{
	return fov;
}

CorE::math::Mat4x4 CorE::Windowing::Window::getProjMat() const
{
	return proj_mat;
}

void CorE::Windowing::Window::setContextCurrent()
{
	//glfwMakeContextCurrent(handle);
}

void CorE::Windowing::Window::setSize(unsigned int w, unsigned int h)
{
	//glfwSetWindowSize(handle, w, h);
	this->width = w;
	this->height = h;
}

void CorE::Windowing::Window::setPos(unsigned int x, unsigned int y)
{
	//glfwSetWindowPos(handle, x, y);
	this->x_pos = x;
	this->y_pos = y;
}

/*
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
*/

void CorE::Windowing::Window::setNearClip(float z_near)
{
	this->z_near = z_near;
	refreshProjMat();
}

void CorE::Windowing::Window::setFarClip(float z_far)
{
	this->z_far = z_far;
	refreshProjMat();
}

void CorE::Windowing::Window::setFieldOfView(float fov)
{
	this->fov = fov;
	refreshProjMat();
}
