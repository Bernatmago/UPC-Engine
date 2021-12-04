#pragma once
#include "Module.h"

#include "il.h"
#include "glew.h"

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

	Texture Load(const char* path);
	void Bind(unsigned id, unsigned slot= GL_TEXTURE0);
	void Unbind(unsigned slot = GL_TEXTURE0);

	inline const short GetDevilVersion() const { return devil_version; }
	
	void OptionsMenu();

private:
	const short devil_version = IL_VERSION;
	unsigned int LoadImg(const char* path);
	void DeleteImg(unsigned& img_id);
};

