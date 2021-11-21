#pragma once
#include "Module.h"
#include "Globals.h"

#include "SDL.h"

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status Update();
	bool CleanUp();

	const unsigned GetKey(SDL_Scancode key) const;
	const bool GetMouseButton(int button) const;
	const void GetMouseDelta(int& x, int& y) const;

private:
	const Uint8 *keyboard = NULL;
	Uint32 mouse;
	int mouse_x, mouse_y;
	int mouse_delta_x, mouse_delta_y;
};