#pragma once
#include "Application.h"
#include "Console.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleGui.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleDebugDraw.h"

using namespace std;

Application::Application()
{	
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());	
	// TODO: Add more modules
	// Mesh
	// Textures	
	modules.push_back(input = new ModuleInput());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(gui = new ModuleGui());
	modules.push_back(debug = new ModuleDebugDraw());
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	delta = 0;
	timer = PerformanceTimer();
	timer.Start();
	prev_tick_time = timer.Read();
	return ret;
}

update_status Application::Update()
{
	double tick_time = timer.Read();
	delta = tick_time - prev_tick_time;
	prev_tick_time = tick_time;
	update_status ret = UPDATE_CONTINUE;
	LOG("%f", delta)
	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update((float)delta);

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	timer.Stop();

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}
