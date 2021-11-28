#pragma once
#include "Module.h"

#include <string>

struct Texture {
	unsigned id;
	std::string path;
	unsigned width;
	unsigned height;
};

class ModuleTextures : 
	public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init() override;
	bool CleanUp() override;

	Texture Load(const char* path);
private:
	unsigned int LoadImg(const char* path);
	void DeleteImg(unsigned& img_id);
};

