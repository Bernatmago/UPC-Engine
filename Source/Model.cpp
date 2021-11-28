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

void Model::Load(const std::string& model_path)
{
	if (loaded)
		CleanUp();

	matrix = float4x4::identity; // Reset on load

	path = model_path.substr(0, model_path.find_last_of("/\\") + 1);
	file_name = model_path.substr(model_path.find_last_of("/\\") + 1);
	name = file_name.substr(0, std::string::size_type(file_name.find_last_of('.')));
	
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(model_path, aiProcess_Triangulate);
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
		
		// Atm we only support loading a single texture so index is hardcoded to 0
		static const int index = 0;
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, index, &file) == AI_SUCCESS)
		{
			
			std::string model_texture_path(file.data);
			std::string texture_file = model_texture_path.substr(model_texture_path.find_last_of("/\\") + 1);
			std::string default_path("Textures\\");

			Texture texture;
			texture = App->textures->Load(file.data);		
			if (!texture.loaded) {
				LOG("Failed to load texture from model params: %s", texture.path.c_str())
				texture = App->textures->Load((path + texture_file).c_str());
			}				
			if (!texture.loaded)
				LOG("Failed to load texture from model file path: %s", texture.path.c_str())
				texture = App->textures->Load((default_path + texture_file).c_str());
			if (!texture.loaded) {
				LOG("Failed to load texture from default: %s", texture.path.c_str())
				LOG("Could not find texture %s")
				return;
			}				
			textures.push_back(texture);
			LOG("Loaded texture from %s", texture.path.c_str())
		}
	}
}

void Model::LoadMeshes(const aiScene* scene)
{
	// Create vbo using mVertices, mTextureCoords and mNumVertices
	textures.reserve(scene->mNumMeshes);
	for (unsigned i = 0; i < scene->mNumMeshes; i++)
	{
		LOG("Loaded mesh %d", i);
		meshes.push_back(Mesh());
		meshes[i].Load(scene->mMeshes[i]);
	}
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
	ImGui::Text("Name: %s", name.c_str());
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
