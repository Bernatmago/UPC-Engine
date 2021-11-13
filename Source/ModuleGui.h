#pragma once
#include "Module.h"
#include "Globals.h"

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

private:

};
