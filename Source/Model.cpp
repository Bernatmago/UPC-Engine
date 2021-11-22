#include "Globals.h"
#include "Model.h"
#include "Application.h"

#include "glew.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "il.h"
#include "ilu.h"

unsigned int LoadSingleImage(const char* path)
{
	ilInit();
	ILuint name; // The image name to return.
	ilGenImages(1, &name); // Grab a new image name.
	ilBindImage(name);
	ilLoadImage(path);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	iluRotate(180.0f);

	return name;
}

Model::Model()
{
}

Model::~Model()
{
}

void Model::Draw()
{
	assert(loaded == true);
	for (Mesh& mesh : meshes) {
		mesh.Draw(textures);
	}
}

void Model::Load(const char* file_name)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file_name, aiProcess_Triangulate);
	if (scene)
	{		
		LoadTextures(scene);
		LoadMeshes(scene);
		loaded = true;
	}
	else
	{
		LOG("Error loading %s: %s", file_name, aiGetErrorString());
	}
}

void Model::LoadTextures(const aiScene* scene)
{
	aiString file;
	textures.reserve(scene->mNumMaterials);
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		// Atm we only support a single texture so index is hardcoded to 0
		static const int index = 0;
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, index, &file) == AI_SUCCESS)
		{
			// TODO: Move stuff to textures module and change to store similar to struct
			textures.push_back(LoadTexture(file.data).id);
		}
	}
}

void Model::LoadMeshes(const aiScene* scene)
{
	// Create vbo using mVertices, mTextureCoords and mNumVertices
	textures.reserve(scene->mNumMeshes);
	for (unsigned i = 0; i < scene->mNumMeshes; i++)
	{
		LOG("Push mesh %d", i);
		meshes.push_back(Mesh());
		meshes[i].Load(scene->mMeshes[i]);
	}
	
}

Texture Model::LoadTexture(const char* path)
{
	Texture texture;
	texture.path = path;
	unsigned int img_id = LoadSingleImage(path);	

	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
		ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		ilGetData());

	ilDeleteImages(1, &img_id);
	return texture;
}

void Model::CleanUp()
{
	assert(loaded == true);
	for (Mesh& mesh : meshes) {
		mesh.CleanUp();
	}
	for (unsigned texture_id : textures) {
		glDeleteTextures(1, &texture_id);
	}
	loaded = false;
}