
#include <iostream>

#include "CorE/window_manager.hpp"
#include "CorE/core_manager.hpp"
#include "CorE/matrix.hpp"


CorE::Windowing::Window::Window() {}

#if (CORENGINE_PLATFORM == CORENGINE_ANDROID)
void initAndroidSurface(ANativeWindow* p_window);

#elif (CORENGINE_PLATFORM == CORENGINE_LINUX_WAYLAND)
void initWaylandSurface(wl_display* p_display, wl_surface* p_surface);

#elif (CORENGINE_PLATFORM == CORENGINE_WINDOWS)

CorE::Windowing::Window::Win32WindowCreateInfo::Win32WindowCreateInfo(DWORD window_style, LPCSTR window_name,
	DWORD class_style, LPCSTR class_name)
	:window_style(window_style),
	window_name(window_name),
	class_style(class_style),
	class_name(class_name) {}

CorE::Windowing::Window::Window(WindowProperties* p_props, Win32WindowCreateInfo* p_create_info) :
	title(p_props->title),
	width(p_props->width),
	height(p_props->height),
	v_sync(p_props->v_sync),
	fov(p_props->fov),
	proj_mat(p_props->proj_mat),
	x_pos(p_props->x_pos),
	y_pos(p_props->y_pos),
	z_near(p_props->z_near),
	z_far(p_props->z_far)

{

	hinstance = GetModuleHandle(NULL);

	WNDCLASS wclass{};
	wclass.style = p_create_info->class_style;
	wclass.lpfnWndProc = DefWindowProc;
	wclass.cbClsExtra = 0;
	wclass.cbWndExtra = 0;
	wclass.hInstance = hinstance;
	wclass.lpszClassName = p_create_info->class_name;
	ATOM res = RegisterClass(&wclass);
	if (!res)
	{
		throw std::runtime_error("Failed to register window class.");
	}
	wndclass = wclass;

	hwnd = CreateWindow(p_create_info->class_name, p_create_info->window_name, p_create_info->window_style, 500, 500, 500, 500,
		nullptr, nullptr, hinstance, nullptr);
	if (!hwnd)
	{
		throw std::runtime_error("Failed to create HWND window handle.");
	}
	
	CorE::Application::checkVkInstance();
	SetWindowPos(hwnd, nullptr, x_pos, y_pos, width, height, SWP_SHOWWINDOW);


	VkWin32SurfaceCreateInfoKHR info{};
	info.hwnd = hwnd;
	info.hinstance = hinstance;

	ensureVkSuccess(vkCreateWin32SurfaceKHR(CorE::Application::instance, &info, nullptr, &vk_surface),
		"Failed to create Win32 surface.");
	
}

CorE::Windowing::Window::~Window()
{
	vkDestroySurfaceKHR(CorE::Application::instance, vk_surface, nullptr);
	DestroyWindow(hwnd);
}


#elif (CORENGINE_PLATFORM == CORENGINE_LINUX_XLIB)
void initXLibSurface(Display* p_display, Window window);

#elif defined CORENGINE_USE_PLATFORM_DIRECTFB
void initDirectFBSurface(IDirectFB* p_interface, IDirectFBSurface* p_surface);

#elif defined CORENGINE_USE_PLATFORM_XRANDR
void initXRandRSurface();

#elif defined CORENGINE_USE_PLATFORM_GGP
void initGoogleGamesSurface(GgpStreamDescriptor stream_descriptor);

#elif defined CORENGINE_USE_PLATFORM_IOS
void initIOSSurface();

#elif defined CORENGINE_USE_PLATFORM_MACOS
void initMacOSSurface();

#elif defined CORENGINE_USE_PLATFORM_OHOS
void initOpenHarmonySurface();

#elif defined CORENGINE_USE_PLATFORM_VI
void initVISurface();

#elif defined CORENGINE_USE_PLATFORM_FUCHSIA
void initFuchsiaSurface(zx_handle_t image_pipe);

#elif defined CORENGINE_USE_PLATFORM_METAL
void initMetalSurface();

#elif defined CORENGINE_USE_PLATFORM_QNX
void initQNXSurface();

#endif


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
