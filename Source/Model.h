#pragma once

#include "assimp/material.h"
#include "assimp/scene.h"
#include "assimp/mesh.h"

#include <vector>

class Model
{
public:
	void Load(const char* file_name);
	void LoadMaterial(const aiScene* scene);
private:
	//std::vector<aiMaterial> materials;
};

