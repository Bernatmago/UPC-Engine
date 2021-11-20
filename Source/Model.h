#pragma once

#include "assimp/material.h"
#include "assimp/scene.h"
#include "assimp/mesh.h"

#include <vector>

struct Texture {
	unsigned int id;	
	std::string path;
	//std::string type;
};

class Model
{
public:
	void Load(const char* file_name);
	void LoadTextures(const aiScene* scene);
	void LoadMeshes(const aiScene* scene);
	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	Texture LoadTexture(const char* path);
private:
	std::vector<Texture> textures;
};

