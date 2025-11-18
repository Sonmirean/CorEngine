#pragma once

#include <thread>
#include <stdexcept>

#include "CorE/corengine.hpp"

#include "CorE/debug.hpp"
#include "CorE/short_type.hpp"
#include "CorE/data_types.hpp"
#include "CorE/matrix.hpp"

namespace CorE
{
	namespace Windowing
	{

		/**
		* Declares a set of properties to be used while creating a Window object.
		*
		* @param title UTF-8 encoded window title.
		* @param width Desired width of the window (in pixels). Must be greater than zero.
		* @param height Desired height of the window (in pixels). Must be greater than zero.
		* @param v_sync Enable vertical synchronization or not.
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
			float				   fov = 1.0471975511965976f;
			CorE::math::Mat4x4	   proj_mat;
			unsigned int		   x_pos = 500;
			unsigned int		   y_pos = 500;
			float				   z_near = 0.01f;
			float				   z_far = 1000.0f;
		};

		/*
		* This class represents a window object.
		* 
		*/
		struct Window
		{
		public:

			// default constructor, do not use
			Window();

			// Both constructor and destructor of the class are platform-dependent.

			#ifdef CORENGINE_USE_PLATFORM_ANDROID
			void initAndroidSurface(ANativeWindow* p_window);

			#elif defined CORENGINE_USE_PLATFORM_WAYLAND
			void initWaylandSurface(wl_display* p_display, wl_surface* p_surface);

			#elif defined CORENGINE_USE_PLATFORM_WIN32
			// This struct can be used to initialize CorE::Windowing::Window
			//
			// TODO - add support for extended window styles
			struct Win32WindowCreateInfo
			{
			public:

				Win32WindowCreateInfo(DWORD window_style, LPCSTR window_name, DWORD class_style,
					LPCSTR class_name);

				DWORD window_style;
				LPCSTR window_name;
				DWORD class_style;
				LPCSTR class_name;

			};

			Window(WindowProperties* p_props, Win32WindowCreateInfo* p_create_info);
			~Window();

			#elif defined CORENGINE_USE_PLATFORM_XCB
			void initXCBSurface(xcb_connection_t* p_connection, xcb_window_t window);

			#elif defined CORENGINE_USE_PLATFORM_XLIB
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
			CorE::math::Mat4x4 getProjMat() const;

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

			uint32_t engine_id;

			const char*		   title;
			unsigned int	   width;
			unsigned int	   height;
			unsigned int 	   x_pos;
			unsigned int	   y_pos;
			bool			   v_sync;
			CorE::math::Mat4x4 proj_mat;
			float			   fov;
			float			   z_near;
			float			   z_far;

			VkSurfaceKHR vk_surface;

			#ifdef CORENGINE_USE_PLATFORM_ANDROID
			ANativeWindow* p_android_window_handle;

			#elif defined CORENGINE_USE_PLATFORM_WAYLAND
			wl_display* p_wayland_display;
			wl_surface* p_wayland_surface;

			#elif defined CORENGINE_USE_PLATFORM_WIN32
			HWND hwnd;
			HINSTANCE hinstance;
			WNDCLASS wndclass;

			#elif defined CORENGINE_USE_PLATFORM_XCB
			xcb_connection_t* p_xcb_server_connection;
			xcb_window_t	  xcb_window;

			#elif defined CORENGINE_USE_PLATFORM_XLIB
			Display* p_xlib_display;
			Window	 xlib_window;

			#elif defined CORENGINE_USE_PLATFORM_DIRECTFB
			IDirectFB*		  p_directfb_interface;
			IDirectFBSurface* p_directfb_surface;

			#elif defined CORENGINE_USE_PLATFORM_XRANDR

			#elif defined CORENGINE_USE_PLATFORM_GGP
			GgpStreamDescriptor google_games_stream_descriptor;

			#elif defined CORENGINE_USE_PLATFORM_IOS
			const void* p_ios_view_object;

			#elif defined CORENGINE_USE_PLATFORM_MACOS
			const void* p_macos_view_object;

			#elif defined CORENGINE_USE_PLATFORM_OHOS
			OHNativeWindow* p_openharmony_window;

			#elif defined CORENGINE_USE_PLATFORM_VI
			void* p_vi_window;

			#elif defined CORENGINE_USE_PLATFORM_FUCHSIA
			zx_handle_t fuchsia_image_pipe;

			#elif defined CORENGINE_USE_PLATFORM_METAL
			CAMetalLayer* p_metal_layer;

			#elif defined CORENGINE_USE_PLATFORM_QNX
			struct _screen_context* p_qnx_context;
			struct _screen_window*  p_qnx_window;

			#endif

		};
	}
	
}