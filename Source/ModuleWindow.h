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
	void SetVsync(bool vsync);

	void OptionsMenu();

	inline SDL_Surface* GetScreenSurface() const { return screen_surface; }
	inline SDL_Window* GetWindow() const { return window; }
	
private:
	void GetMonitorResolution(int& width, int&height);

	SDL_Window* window = NULL;
	SDL_Surface* screen_surface = NULL;

	bool fullscreen;
	bool resizable;
	int width;
	int max_width;
	int max_height;
	int height;
	int refresh_rate;
	bool vsync;
};

#endif // __ModuleWindow_H__