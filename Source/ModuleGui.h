#pragma once
#include "Module.h"
#include "Globals.h"
#include "Console.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleGui : public Module
{
public:
	ModuleGui();
	~ModuleGui();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void RenderMenu();
	void RenderSidebar();

	//TODO
	//fps graph
	//window options
	//hardware detection
	//about
	

private:
	bool show_console = false;
};


