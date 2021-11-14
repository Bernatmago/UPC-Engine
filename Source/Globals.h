#pragma once
#include <windows.h>
#include <stdio.h>

#include "Console.h"

//#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

# define LOG(format, ...) Logger->AddLog(__FILE__, __LINE__, format, __VA_ARGS__);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1080
#define SCREEN_MAX_WIDTH 1920
#define SCREEN_HEIGHT 720
#define SCREEN_MAX_HEIGHT 1080
#define FULLSCREEN false
#define RESIZABLE false
#define VSYNC true
#define TITLE "Super Awesome Engine"