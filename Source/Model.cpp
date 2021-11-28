#include "Globals.h"
#include "Model.h"
#include "Application.h"

#include "glew.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "il.h"
#include "ilu.h"
#include "imgui.h"

unsigned int LoadSingleImage(const char* path)
{
	ilInit();
	ILuint name; // The image name to return.
	ilGenImages(1, &name); // Grab a new image name.
	ilBindImage(name);
	ilLoadImage(path);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	iluRotate(180.0f);

	return name;
}

Model::Model()
{
}

Model::~Model()
{
}

void Model::Draw()
{
	assert(loaded == true);
	for (Mesh& mesh : meshes) {
		mesh.Draw(matrix, textures);
	}
}

void Model::Load(const std::string& path)
{
	if (loaded) {
		CleanUp();
		loaded = true;
	}
	matrix = float4x4::identity; // Reset on load

	file_name = path.substr(path.find_last_of("/\\") + 1);
	name = file_name.substr(0, std::string::size_type(file_name.find_last_of('.')));
	
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
	if (scene)
	{		
		LoadTextures(scene);
		LoadMeshes(scene);
		loaded = true;
	}
	else
	{
		LOG("Error loading %s: %s", file_name, aiGetErrorString());
	}
}

void Model::LoadTextures(const aiScene* scene)
{
	aiString file;
	textures.reserve(scene->mNumMaterials);
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		// Atm we only support a single texture so index is hardcoded to 0
		static const int index = 0;
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, index, &file) == AI_SUCCESS)
		{
			// TODO: Move stuff to textures module and change to store similar to struct
			textures.push_back(LoadTexture(file.data));
		}
	}
}

void Model::LoadMeshes(const aiScene* scene)
{
	// Create vbo using mVertices, mTextureCoords and mNumVertices
	textures.reserve(scene->mNumMeshes);
	for (unsigned i = 0; i < scene->mNumMeshes; i++)
	{
		LOG("Push mesh %d", i);
		meshes.push_back(Mesh());
		meshes[i].Load(scene->mMeshes[i]);
	}
	
}

Texture Model::LoadTexture(const char* path)
{
	Texture texture;
	texture.path = path;
	// TODO: Try to find the texture (in order)
	// Path described by fbx
	// Same folder than fbx
	// Textures folder (workdir)
	unsigned int img_id = LoadSingleImage(path);

	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), texture.width=ilGetInteger(IL_IMAGE_WIDTH),
		texture.height=ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		ilGetData());

	ilDeleteImages(1, &img_id);
	return texture;
}

void Model::CleanUp()
{
	assert(loaded == true);
	for (Mesh& mesh : meshes) {
		mesh.CleanUp();
	}
	meshes.clear();
	for (Texture texture : textures) {
		glDeleteTextures(1, &texture.id);
	}
	textures.clear();
	loaded = false;
}

const float3& Model::GetPosition() const
{
	return matrix.TranslatePart();
}

void Model::OptionsMenu()
{
	static bool locked = true;
	float3 position = matrix.TranslatePart(); //Equal to col3(3)
	float3 scale = matrix.ExtractScale();
	
	// TODO: Move in world coords
	ImGui::Text("Translation");
	if (ImGui::SliderFloat3("t.XYZ", &position[0], -5.0f, 5.0f))
		matrix.SetTranslatePart(position);
	ImGui::Separator();

	ImGui::Text("Scale");
	ImGui::Checkbox("Lock", &locked);
	ImGui::SameLine();
	float3 scale_delta = scale;
	if (ImGui::SliderFloat3("s.XYZ", &scale[0], 0.5, 5.0f)) {
		if (!locked) {
			matrix.scaleX = scale[0];
			matrix.scaleY = scale[1];
			matrix.scaleZ = scale[2];
		}
		else {
			scale_delta -= scale;
			// TODO: Check the increment symbol position stuff
			for (int i = 0; i < 3; i++) {
				if (scale_delta[i] != 0.0f) {
					LOG("Changed scale %d", i)
					matrix.scaleX = scale[i];
					matrix.scaleY = scale[i];
					matrix.scaleZ = scale[i];
					break; // Only one axis can change
				}
			}
		}
	}
	// TODO: Add rotation	
}

void Model::PropertiesWindow(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(250, 200), ImGuiCond_Once);
	ImGui::Begin("Model Properties", p_open); // TODO: Fill with filename
	// TODO: Add path
	ImGui::Text("Texture");
	for (int i = 0; i < textures.size(); ++i) {
		const Texture& texture = textures[i];
		ImGui::Text("T[%d]: %s %dx%d", i, texture.path.c_str(), texture.width, texture.height);
	}
	ImGui::Separator();
	ImGui::Text("Meshes");

	int vertices = 0;
	int indexes = 0;
	for (int i = 0; i < meshes.size(); ++i) {
		const Mesh& mesh = meshes[i];
		ImGui::Text("M[%d]: %dt, (%dv, %di)", i, mesh.GetNumIndexes()/3, mesh.GetNumVertices(), mesh.GetNumIndexes());
		indexes += mesh.GetNumIndexes();
		vertices += mesh.GetNumVertices();
	}
	ImGui::Text("Total : %dt, (%dv, %di)", indexes/3, vertices, indexes);

	ImGui::End();

}
