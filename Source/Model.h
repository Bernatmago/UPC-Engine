#pragma once

#include "Mesh.h"

#include "assimp/scene.h"

#include <vector>

struct Texture {
	unsigned id;	
	std::string path;
	//std::string type;
};

class Model
{
public:
	Model(const char* file_name);
	~Model();
	void Draw();
private:
	void Load(const char* file_name);
	void LoadTextures(const aiScene* scene);
	void LoadMeshes(const aiScene* scene);
	Texture LoadTexture(const char* path);
	std::vector<unsigned> textures;
	std::vector<Mesh> meshes;
};

