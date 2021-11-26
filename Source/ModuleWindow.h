#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();
	~ModuleWindow();

	bool Init() override;
	bool CleanUp() override;

	void WindowResized();
	void SetFullScreen(bool fullscreen);
	void SetResizable(bool resizable);
	void SetSize(int w, int h);

	void OptionsMenu();

	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;
	
	int refresh_rate;
};

#endif // __ModuleWindow_H__