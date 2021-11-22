#pragma once
#include "SDL.h"

class Timer
{
public:
	void Start();
	double Read();
	double Stop();
private:
	bool running = false;
	uint32_t start_time;
	double current_time;
};


class PerformanceTimer
{
public:
	void Start();
	double Read();
	double Stop();
private:
	bool running = false;
	uint64_t start_time;
	double current_time;
};

