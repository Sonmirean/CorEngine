#pragma once

#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Debug.h"
#include "Types.h"

/**
* Declares a set of properties to be used while creating a Window object.
*
* @param title UTF-8 encoded window title.
* @param width Desired width of the window (in pixels). Must be greater than zero.
* @param height Desired height of the window (in pixels). Must be greater than zero.
* @param v_sync Enable vertical synchronization or not.
* @param monitor The monitor to use for full screen mode, or `nullptr` for windowed mode.
* @param share The window whose context to share resources with, or `nullptr` to not share resources.
* @param fov Field of View parameter of the window, or `nullptr`. Must be given in radians.
* @param proj_mat A projection matrix to be used by the window, or `nullptr`.
* @param x_pos Desired X position after creation.
* @param y_pos Desired Y position after creation.
* @param z_near Near-clip value to be used by the window, or `nullptr`.
* @param z_far Far-clip value to be used by the window, or `nullptr`.
*/
struct WindowProperties
{
	const char*			   title = nullptr;
	unsigned int		   width = 500;
	unsigned int		   height = 300;
	bool				   v_sync = true;
	//GLFWmonitor*		   monitor = nullptr;
	//GLFWwindow*			   share = nullptr;
	float				   fov = 1.0471975511965976f;
	glm::mat4x4			   proj_mat{};
	unsigned int		   x_pos = 500;
	unsigned int		   y_pos = 500;
	float				   z_near = 0.01f;
	float				   z_far = 1000.0f;
};

/*
This class represents a window object.

There are stored essential rendering parameters,
such as near and far clip planes distance,
field of view, projection matrix, etc.

Accepts any GLFW window hints called before creation.

*/
class Window
{
public:

	Window(WindowProperties* props);

	~Window();

	/*
	bool operator ==(Window win);
	*/

	// Refreshes projection matrix of this window.
	void refreshProjMat();

	// Moves window to the center of display.
	void centralize();

	// General update method.
	void update();

	// Swaps window buffers.
	void swapBuffers();

	// Polls window events.
	//void pollEvents();

	// Sets resizing limits for this window.
	//void setLimits(unsigned int min_w, unsigned int min_h, unsigned int max_w, unsigned int max_h);

	/////////////////////////
	///      GETTERS      ///
	/////////////////////////

	// Returns pointer to the title of this window.
	const char* getTitle() const;
	// Gets coordinates of the top left corner of this window.
	unsigned int getPos(char dim) const;
	// Gets window size.
	unsigned int getSize(char dim) const;
	// Checks whether vertical synchronization is enabled for this window.
	bool isVSync() const;
	// Checks whether window should close.
	//bool shouldClose() const;
	// Returns pointer to GLFWwindow base of this window.
	//GLFWwindow* getHandle() const;
	// Returns pointer to window, with which this window's context is shared.
	//GLFWwindow* getShare() const;
	// Returns monitor to use for fullscreen mode, if any.
	//GLFWmonitor* getMonitor() const;
	// Gets the near clip (z_near) of this window.
	float getNearClip() const;
	// Gets the far clip (z_far) of this window.
	float getFarClip() const;
	// Gets the Field of View parameter of this window.
	float getFieldOfView() const;
	// Gets projection matrix of this window.
	glm::mat4x4 getProjMat() const;

	/////////////////////////
	///      SETTERS      ///
	/////////////////////////

	// Makes window's context current.
	void setContextCurrent();
	// Changes the size of this window.
	void setSize(unsigned int w, unsigned int h);
	// Changes the position of this window.
	void setPos(unsigned int x, unsigned int y);
	// Enables or disables VSync for this window.
	//void setVSync(bool b);
	// Sets window title.
	//void setTitle(const char* title);
	// Sets shouldClose parameter of the window.
	//void setShouldClose(bool b);
	// Sets the near-clip (z_near) parameter of this window
	// and refreshes its projection matrix.
	void setNearClip(float z_near);
	// Sets the far-clip (z_far) parameter of this window
	// and refreshes its projection matrix.
	void setFarClip(float z_far);
	// Changes the field of view of this window
	// and refreshes its projection matrix.
	void setFieldOfView(float fov);


private:

	friend struct Application;

	uint32_t id;

	VkSurfaceKHR vk_surface;

	//GLFWwindow*  handle;
	//GLFWwindow*	 share;
	const char*  title;
	unsigned int width;
	unsigned int height;
	unsigned int x_pos;
	unsigned int y_pos;
	bool		 v_sync;
	glm::mat4x4  proj_mat;
	float		 fov;
	float		 z_near;
	float		 z_far;

};
