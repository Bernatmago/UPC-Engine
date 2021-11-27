#pragma once
#include "Module.h"
#include "Globals.h"
#include "Model.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init() override;
	update_status PreUpdate() override;
	update_status Update(const float delta) override;
	update_status PostUpdate() override;
	bool CleanUp() override;

	void WindowResized(unsigned width, unsigned height);

	void* context;
	Model* model = nullptr;
};
