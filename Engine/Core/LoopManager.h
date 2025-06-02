#pragma once

#include <chrono>
#include <future>
#include "WindowManager.h"

/**
* Declares a set of properties to be used while creating a Heart object.
*
* 
*/
struct HeartProperties
{
	unsigned int fps_cap = 60;
	vec<Window*> p_attached_windows;
};

// This class represents an asynchronous rendering cycle.
// It must have at least one window attached.
// 
class Heart
{
public:

	Heart(HeartProperties* props);

	~Heart();

	virtual void init();

	// Starts the rendering cycle.
	// Note that starting it putting thread in which it was runned in a 100-millisecond sleep.
	// This is done to prevent some inheritance bug, which is caused by vtables (I don't remember details, sorry).
	void start();
	void stop();

	virtual void update();
	virtual void input();
	virtual void render();


	float getDelta();
	unsigned int getFps();

	void setFpsCap(unsigned int cap);
	unsigned int getFpsCap();

	bool isRunning() const;

private:

	void run();

	std::future<void> async_call;

	bool is_running = false;
	unsigned int fps_cap = 60;

	unsigned int fps = 0;
	float delta = 0;

	std::vector<Window*> p_attached_windows;

	unsigned const long NANOSECOND = 1000000000L;
	const long FRAMERATE = 1000L;
	const float FRAMETIME = 1.0f / FRAMERATE;

};