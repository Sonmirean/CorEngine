
#define GLFW_INCLUDE_VULKAN

#include <iostream>
#include <thread>
#include <cstring>

#include <chrono>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "AppStructure.h"

#include "CorEngine.h"
#include "Loader.h"


class SpecialHeart : public Heart
{

public:

	using Heart::Heart;

	void render() override
	{
		
	}
};


int main()
{
	glfwInit();
	
	char appname[7] = "damnit";
	uint32_t appver[4] = { 1,2,3,4 };

	Application::initVulkan(appname, appver);

	char title[] = "Stained glass window";
	WindowProperties win_props
	{
		.title = title
	};
	
	glfwDefaultWindowHints();
	Window stained_glass_window = Window(&win_props);

	HeartProperties heart_props;
	heart_props.p_attached_windows.push_back(&stained_glass_window);
	SpecialHeart demonheart = SpecialHeart(&heart_props);

	demonheart.start();

	while (demonheart.isRunning())
	{
		stained_glass_window.update();
	}
	
	Application::finalCleanup();
	return EXIT_SUCCESS;
}

