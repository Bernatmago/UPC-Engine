#pragma once

#include "assimp/scene.h"

#include <vector>


class Mesh
{
public:
	
	Mesh(const aiMesh* mesh);
	
	~Mesh();
	
	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();

	void Draw(const std::vector<unsigned>& model_textures);
	
	unsigned vbo;
	unsigned num_vertices;
	unsigned ebo;
	unsigned num_indices;
	unsigned vao;
	unsigned texture_index;;
};

