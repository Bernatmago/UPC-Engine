#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleWindow;
class ModuleRender;
class ModuleGui;
class ModuleInput;
class ModuleShader;

class ModuleRenderExercise;
class ModuleTextures;

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
	ModuleShader* shader = nullptr;

private:

	std::list<Module*> modules;

};

extern Application* App;
