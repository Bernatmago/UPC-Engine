#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"
#include "Timer.h"

class ModuleWindow;
class ModuleRender;
class ModuleGui;
class ModuleInput;
class ModuleProgram;
class ModuleCamera;
class ModuleDebugDraw;
class ModuleTextures;

class Console;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	double GetDeltaTime() { return delta; };

public:
	ModuleWindow* window = nullptr;
	ModuleRender* renderer = nullptr;
	ModuleGui* gui = nullptr;
	ModuleInput* input = nullptr;
	ModuleProgram* program = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleDebugDraw* debug = nullptr;
	ModuleTextures* textures = nullptr;

private:
	std::list<Module*> modules;
	PerformanceTimer timer;

	double  delta = 0;
	double  prev_tick_time = 0;
};

extern Application* App;
