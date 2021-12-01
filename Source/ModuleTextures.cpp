#include "ModuleTextures.h"

#include "glew.h"
#include "ilu.h"

ModuleTextures::ModuleTextures()
{
}

ModuleTextures::~ModuleTextures()
{
}

bool ModuleTextures::Init()
{
	ilInit();
	return true;
}

bool ModuleTextures::CleanUp()
{
	ilShutDown();
	return true;
}

Texture ModuleTextures::Load(const char* path)
{
	Texture texture;
	texture.path = path;
	unsigned int img_id = LoadImg(path);

	if (img_id != 0) {
		glGenTextures(1, &texture.id);
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), texture.width = ilGetInteger(IL_IMAGE_WIDTH),
			texture.height = ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
			ilGetData());

		DeleteImg(img_id);
		texture.loaded = true; // False by default
	}	
	return texture;
}

unsigned int ModuleTextures::LoadImg(const char* path)
{
	ILuint img_id; // The image name to return.
	ilGenImages(1, &img_id); // Grab a new image name.
	ilBindImage(img_id);
	if (!ilLoadImage(path))
		return 0;
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	iluFlipImage();

	return img_id;
}

void ModuleTextures::DeleteImg(unsigned& img_id)
{
	ilDeleteImages(1, &img_id);
}
