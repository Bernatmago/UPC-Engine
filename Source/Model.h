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
	bool LoadMeshes(const aiScene* scene);
	bool LoadTextures(const aiScene* scene);
	bool LoadTexture(const aiMaterial* texture);

	void UpdateMatrix();
	
	bool loaded = false;
	float4x4 matrix = float4x4::identity;
	float3 position;
	float3 rotation;
	float3 scale;
	std::string name;
	std::string path;
	std::string file_name;
	std::vector<Texture> textures;
	std::vector<Mesh> meshes;
};

