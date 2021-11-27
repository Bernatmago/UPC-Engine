#pragma once

#include "Mesh.h"

#include "assimp/scene.h"
#include "MathGeoLib.h"

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

	void OptionsMenu();

private:
	bool loaded = false;
	float4x4 model = float4x4::identity;
	// TODO: Reload shit
	// TODO: Manage dropping models and paths
	void LoadTextures(const aiScene* scene);
	void LoadMeshes(const aiScene* scene);
	Texture LoadTexture(const char* path);
	std::vector<unsigned> textures;
	std::vector<Mesh> meshes;
};

