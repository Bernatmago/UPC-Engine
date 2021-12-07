#pragma once
#include <windows.h>
#include <stdio.h>

#include "Console.h"
#include "Timer.h"

# define LOG(format, ...) Logger->AddLog(__FILE__, __LINE__, format, __VA_ARGS__);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define WINDOWED_RATIO 0.75f
#define FULLSCREEN false
#define RESIZABLE false
#define TITLE "Super Awesome Engine"

#define M_PI 3.14159265358979323846

constexpr float to_rad = (float)M_PI / 180.0f;
constexpr float to_deg = 180.0f / (float)M_PI;