#pragma once

#include "ModuleTextures.h"

#include "assimp/scene.h"
#include "MathGeoLib.h"

#include <vector>


class Mesh
{
public:
	
	Mesh();	
	~Mesh();

	void Load(const aiMesh* mesh);
	void Draw(float4x4& model, const std::vector<Texture>& model_textures);
	void CleanUp();

	unsigned GetNumVertices() const { return num_vertices; }
	unsigned GetNumIndexes() const { return num_indices; }

private:
	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();
	
	
	bool loaded;
	unsigned vbo;
	unsigned num_vertices;
	unsigned ebo;
	unsigned num_indices;
	unsigned vao;
	unsigned texture_index;
};

