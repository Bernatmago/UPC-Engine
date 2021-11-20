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

public:
	ModuleWindow* window = nullptr;
	ModuleRender* renderer = nullptr;
	ModuleGui* gui = nullptr;
	ModuleInput* input = nullptr;
	ModuleProgram* shader = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleDebugDraw* debug = nullptr;

	Console* console = nullptr;

private:
	std::list<Module*> modules;
};

extern Application* App;
