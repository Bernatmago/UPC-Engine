#pragma once
#include "SDL.h"

#include "Module.h"
#include "Globals.h"
#include "Console.h"


struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

struct AboutData {
	SDL_version sdl_version;
	int n_cpu;
	float ram_gb;
	unsigned char* gpu;
	unsigned char* gpu_brand;
	int vram_budget;
	int vram_free;
};

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

	void Menu();

	// Sidebar
	void Sidebar();
	void Performance();
	void WindowOptions();
	void CameraOptions();
	void About();

	//TODO
	//fps graph
	//window options
	//hardware detection
	//about
private:
	bool show_console = false;
	bool show_demo = false;
	AboutData about;
};


