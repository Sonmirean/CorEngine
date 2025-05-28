
#define GLFW_INCLUDE_VULKAN

#include <iostream>
#include <thread>

#include <chrono>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//#include "AppStructure.h"
#include "WindowManager.h"

#include "CorEngine.h"

/*
class SpecialHeart : public Heart
{

public:

    using Heart::Heart;

    void render() override
    {
       
    }
};
*/

int main()
{
    /*
    glfwInit();

    char name[] = "Window to hell";

    WindowProperties prps;
    prps.width = 400;
    prps.height = 200;
    prps.title = &name[0];
    prps.share = nullptr;
    prps.x_pos = 900;
    prps.y_pos = 400;


    //glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
     
    //Window win1 = Window(&prps);

    HeartProperties heartprops;
    heartprops.fps_cap = 100;
    SpecialHeart demonheart = SpecialHeart(&heartprops);


	unsigned int ver[4] = { 0,0,0,1 };
	AppStructure::initVulkan(&name[0], ver);

    


    demonheart.start();

    
    while (demonheart.isRunning())
    {
        
    }
    
    */
    return EXIT_SUCCESS;
}

