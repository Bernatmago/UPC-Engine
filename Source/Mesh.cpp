#include "Mesh.h"

#include "Application.h"
#include "ModuleProgram.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"

#include "glew.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::Load(const aiMesh* mesh)
{
	texture_index = mesh->mMaterialIndex;
	LoadVBO(mesh);
	LoadEBO(mesh);
	CreateVAO();
	loaded = true;
}

void Mesh::LoadVBO(const aiMesh* mesh)
{
	num_vertices = mesh->mNumVertices;

	// Generate & activate buffer
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// TODO: Static, change to interleaved after it is working
	// Compute sizes, vertex position and texture coords
	unsigned vertex_size = (sizeof(float) * 3 + sizeof(float) * 2);
	unsigned buffer_size = vertex_size * mesh->mNumVertices;
	unsigned position_size = sizeof(float) * 3 * mesh->mNumVertices;
	unsigned uv_offset = position_size;
	unsigned uv_size = sizeof(float) * 2 * mesh->mNumVertices;

	// Update buffer data attributes, nullptr assigns no data
	glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
	// Update buffer subset with vertex positions
	glBufferSubData(GL_ARRAY_BUFFER, 0, position_size, mesh->mVertices);

	// Map a section of a buffer data store to fill it in a custom manner
	float2* uvs = (float2*)(glMapBufferRange(GL_ARRAY_BUFFER, uv_offset, uv_size, GL_MAP_WRITE_BIT));
	for (unsigned i = 0; i < mesh->mNumVertices; ++i)
	{
		// Fill remaining buffer with uwus
		uvs[i] = float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void Mesh::LoadEBO(const aiMesh* mesh)
{
	num_indices = mesh->mNumFaces * 3;

	// Generate & activate buffer
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// Compute size, each face is 3 vertex indexes
	unsigned index_size = sizeof(unsigned) * mesh->mNumFaces * 3;

	// Update buffer data attributes
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, nullptr, GL_STATIC_DRAW);

	// Map buffer and fill it in a custom manner
	unsigned* indices = (unsigned*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
	{
		assert(mesh->mFaces[i].mNumIndices == 3);
		*(indices++) = mesh->mFaces[i].mIndices[0];
		*(indices++) = mesh->mFaces[i].mIndices[1];
		*(indices++) = mesh->mFaces[i].mIndices[2];
	}
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void Mesh::CreateVAO()
{
	// Generate vao, activate & bind buffers
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// Define layout, index, size, type, normalized, stride, pointer
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // Positions
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * num_vertices)); // Texture coords
}

void Mesh::Draw(float4x4& model, const std::vector<Texture>& model_textures)
{
	assert(loaded == true);
	unsigned program_id = App->program->program_id;
	
	glUseProgram(program_id);
	glUniformMatrix4fv(glGetUniformLocation(program_id, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program_id, "view"), 1, GL_TRUE, &App->camera->GetView()[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program_id, "proj"), 1, GL_TRUE, &App->camera->GetProjection()[0][0]);
		
	glActiveTexture(GL_TEXTURE0);	
	glBindTexture(GL_TEXTURE_2D, model_textures[texture_index].id);

	// TODO: Addapt shader, not using diffuse concept
	//glUniform1i(glGetUniformLocation(program_id, "diffuse"), 0);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
}

void Mesh::CleanUp()
{
	assert(loaded == true);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	loaded = false;
}