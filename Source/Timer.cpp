#include "Timer.h"


void Timer::Start()
{
	running = true;
	start_time = SDL_GetTicks();
}

double Timer::Read()
{
	if (running) {
		uint32_t now = SDL_GetTicks();
		current_time = (double)(now - start_time); // ms
	}
	return current_time;
}

double Timer::Stop()
{
	return Read();
	running = false;
}


void PerformanceTimer::Start()
{
	running = true;
	start_time = SDL_GetPerformanceCounter();
}

double PerformanceTimer::Read()
{
	static const double frequency = (double)SDL_GetPerformanceFrequency();
	if (running) {
		uint64_t now = SDL_GetPerformanceCounter();
		current_time = (double)((now - start_time) * 1000.0 / frequency);
	}
	return current_time;
}

double PerformanceTimer::Stop()
{
	return Read();
	running = false;
}
