#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleWindow;
class ModuleRender;
class ModuleGui;
class ModuleInput;
class ModuleProgram;
class ModuleCamera;
class ModuleDebugDraw;

class ModuleTextures; // TODO: move textures here

class Console;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	inline uint32_t GetDeltaTime() { return delta; };

public:
	ModuleWindow* window = nullptr;
	ModuleRender* renderer = nullptr;
	ModuleGui* gui = nullptr;
	ModuleInput* input = nullptr;
	ModuleProgram* program = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleDebugDraw* debug = nullptr;

	Console* console = nullptr;

private:
	std::list<Module*> modules;

	uint32_t  delta = 0;
	uint32_t  prev_tick_time = 0;
};

extern Application* App;
