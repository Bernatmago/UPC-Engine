#pragma once
#include "Module.h"

#include "il.h"

#include <string>

struct Texture {
	bool loaded = false;
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

	inline const short GetDevilVersion() const { return devil_version; }

	Texture Load(const char* path);
private:
	const short devil_version = IL_VERSION;
	unsigned int LoadImg(const char* path);
	void DeleteImg(unsigned& img_id);
};

