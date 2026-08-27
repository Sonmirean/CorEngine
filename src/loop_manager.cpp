
#include <iostream>
#include <chrono>
#include <future>

#include "CorE/window_manager.hpp"
#include "CorE/loop_manager.hpp"


CorE::Heart::Heart(HeartProperties* props) :
	fps_cap(props->fps_cap)
{

}

CorE::Heart::~Heart()
{

}

void CorE::Heart::init()
{

}

void CorE::Heart::start()
{
	if (!is_running)
	{
		async_call = std::async(std::launch::async, &Heart::run, this);

		// delay for slow ass vtables
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

}

void CorE::Heart::stop()
{
	if (is_running)
	{
		this->is_running = false;
	}
}

void CorE::Heart::run()
{

	is_running = true;

	// Why do we need frames_processed variable?
	/// To know how much frames we
	/// did processed per this second.
	unsigned int frames_processed = 0;
	// Why do we need nanos_counter variable?
	/// To know when the second of real time is passed.
	unsigned int nanos_counter = 0;
	// Why do we need unprocessed time?
	/// 
	double unprocessed_time = 0;

	auto loop_start_time = std::chrono::steady_clock::now();
	auto loop_end_time = std::chrono::steady_clock::now();

	bool rendering = false;

	unsigned int loops_per_frame = 0;

	/// ------------------------------- /// LOOP /// ------------------------------- ///
	while (is_running)
	{
		/// SUMMARY OF THE PREVIOUS LOOP ///
		//// Summary calculations are kept outside the loop.
		// Time passed to process a frame (in nanos).
		float passed_to_frame = static_cast<float>(loop_end_time.time_since_epoch().count() - loop_start_time.time_since_epoch().count());
		// Same thing, but in seconds.
		delta = passed_to_frame / static_cast<float>(NANOSECOND);
		/// SUMMARY OF THE PREVIOUS LOOP ///


		///         \/ \/ \/         ///
		///   START  OF  THE  LOOP   ///
		// Note that the end of the previous
		// loop is taken as the start time.
		// It is done to exclude time spent
		// to summary calculation from loop (?)
		loop_start_time = loop_end_time;
		///   START  OF  THE  LOOP   ///
		///         \/ \/ \/         ///


		// Adds time spent to previous loop (In seconds)
		unprocessed_time += passed_to_frame / (double)NANOSECOND;

		// Adds time spent to previous loop (In nanos)
		nanos_counter += static_cast<unsigned int>(passed_to_frame);

		// Here, the loop processes all kinds of user input.
		input();


		///= INNER LOOP
		//=== While UNPROCESSED is more than FRAMETIME :::
		while (unprocessed_time > FRAMETIME)
		{

			/// We enable the render, therefore, consider this frame processed.
			rendering = true;

			/// We subtract one FRAMETIME unit from
			/// unprocessed time, leaving it only the surplus
			unprocessed_time -= FRAMETIME;

			// If nanos_counter is more than 1 million, i.e. if
			// the time passed to render previous several frames
			// is more than 1 second :::
			if (nanos_counter >= NANOSECOND)
			{
				/// We may determine FPS, i.e. how much frames we're processed
				/// per said second. Remember, that not every loop processes
				/// a frame, it does only if UNPROCESSED become more than FRAMETIME
				/// in this loop, i.e. if time spent to process several previous
				/// loops is more than 0.001 second.
				fps = frames_processed;

				/// We also do reset both FRAMES and FRAME_COUNTER variables to 0.
				frames_processed = 0;
				nanos_counter = 0;
			}
		}
		//===========================================//


		// If rendering is enabled (i.e. in this loop
		// unprocessed time counter became more than 0.001f),
		/// we call general update() and render() functions, and add
		/// 1 to FRAMES variable, i.e. consider this frame processed and rendered.
		if (rendering)
		{
			update();
			render();
			frames_processed++;
		}


		// Disabling the rendering, because it only need to be
		// enabled if the unprocessed_time became more than 0.001 sec.
		if (!rendering)
		{
			loops_per_frame++;
		}
		else
		{
			loops_per_frame = 0;
			rendering = false;
		}
		


		///        \/ \/ \/        ///
		///   END  OF  THE  LOOP   ///
		loop_end_time = std::chrono::steady_clock::now();
		///   END  OF  THE  LOOP   ///
		///        \/ \/ \/        ///
	}
	/// ------------------------------- /// LOOP /// ------------------------------- ///
}

void CorE::Heart::update()
{

}
void CorE::Heart::input()
{

}
void CorE::Heart::render()
{
	
}


float CorE::Heart::getDelta()
{
	return delta;
}
unsigned int CorE::Heart::getFps()
{
	return fps;
}

void CorE::Heart::setFpsCap(unsigned int cap)
{
	fps_cap = cap;
}
unsigned int CorE::Heart::getFpsCap()
{
	return fps_cap;
}

bool CorE::Heart::isRunning() const
{
	return is_running;
}

