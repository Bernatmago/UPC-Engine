#pragma once

#include "ModuleTextures.h"
#include "Mesh.h"

#include "assimp/scene.h"
#include "MathGeoLib.h"

#include <vector>
#include <string>

class Model
{
public:
	Model();
	~Model();

	void Load(const std::string& file_name);
	void Draw();
	void CleanUp();

	const float3& GetPosition() const;

	void OptionsMenu();
	void PropertiesWindow(bool* p_open);

private:
	bool loaded = false;
	float4x4 matrix = float4x4::identity;
	std::string name;
	std::string path;
	std::string file_name;
	// TODO: Reload shit
	// TODO: Manage dropping models and paths
	void LoadTextures(const aiScene* scene);
	void LoadMeshes(const aiScene* scene);
	Texture LoadTexture(const char* path);
	std::vector<Texture> textures;
	std::vector<Mesh> meshes;
};

