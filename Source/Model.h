#pragma once

#include "ModuleTextures.h"
#include "Mesh.h"

#include "assimp/scene.h"
#include "MathGeoLib.h"

#include <vector>

class Model
{
public:
	Model();
	~Model();

	void Load(const char* file_name);
	void Draw();
	void CleanUp();

	const float3& GetPosition() const;

	void OptionsMenu();
	void PropertiesWindow(bool* p_open);

private:
	bool loaded = false;
	float4x4 model = float4x4::identity;
	// TODO: Reload shit
	// TODO: Manage dropping models and paths
	void LoadTextures(const aiScene* scene);
	void LoadMeshes(const aiScene* scene);
	Texture LoadTexture(const char* path);
	std::vector<Texture> textures;
	std::vector<Mesh> meshes;
};

