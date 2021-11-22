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
	Model();
	~Model();
	void Load(const char* file_name);
	void Draw();
	void CleanUp();
private:
	bool loaded = false;
	
	void LoadTextures(const aiScene* scene);
	void LoadMeshes(const aiScene* scene);
	Texture LoadTexture(const char* path);
	std::vector<unsigned> textures;
	std::vector<Mesh> meshes;
};

