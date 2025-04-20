
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <thread>

#include <chrono>

#include "AppStructure.h"
#include "WindowManager.h"

#include "CorEngine.h"
/*
void foo()
{
    
    unsigned int ver[4] = { 0,0,0,1 };

    AppStructure::initVulkan(&name[0], ver);

    

    AppStructure::addWindow(&windw);

    //std::chrono::duration<int, std::ratio<1, 1000>> duration_ok{ 3000 };

    //std::this_thread::sleep_for(duration_ok);

    AppStructure::cleanup();
}
*/


class SpecialHeart : public Heart
{

public:

    using Heart::Heart;

    void render() override
    {
        printf("smthng is rendered\n");
    }
};

int main()
{

    glfwInit();

    char name[] = "Window to hell";

    WindowProperties prps;
    prps.width = 400;
    prps.height = 200;
    prps.title = &name[0];
    prps.share = nullptr;
    prps.x_pos = 900;
    prps.y_pos = 400;


    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
     
    //Window win1 = Window(&prps);

    HeartProperties heartprops;
    heartprops.fps_cap = 100;
    SpecialHeart demonheart = SpecialHeart(&heartprops);

    

    demonheart.start();

   
    
    while (demonheart.isRunning())
    {
        
    }
    
    
    return EXIT_SUCCESS;
}

