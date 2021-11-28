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

	bool Init() override;
	update_status PreUpdate(const float delta) override;
	bool CleanUp() override;

	const unsigned GetKey(SDL_Scancode key) const;
	const bool GetKeyMod(SDL_Keymod modifier) const;
	const bool GetMouseButton(int button) const;
	const void GetMouseDelta(int& x, int& y) const;
	int GetScrollDelta() const;

private:
	const Uint8 *keyboard = NULL;
	Uint32 mouse;
	SDL_Keymod keymods;
	int mouse_x, mouse_y;
	int mouse_delta_x, mouse_delta_y;
	int scroll_delta;
};