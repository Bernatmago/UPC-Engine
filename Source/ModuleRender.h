#pragma once
#include "Module.h"
#include "Globals.h"
#include "Model.h"

#include <vector>

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init() override;
	update_status PreUpdate(const float delta) override;
	update_status Update(const float delta) override;
	update_status PostUpdate(const float delta) override;
	bool CleanUp() override;

	void WindowResized(unsigned width, unsigned height);

	void PerformanceMenu(const float delta);
	void FpsGraph();
	void AddFrame(const float delta);

	void* context;
	Model* model = nullptr;

private:
	int vram_budget;
	int vram_free;

	static const unsigned n_bins = 25;
	std::vector<float> fps_log;
	std::vector<float> ms_log;

};
