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
	if (loaded) {
		for (Mesh& mesh : meshes) {
			mesh.Draw(matrix, textures);
		}
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
		bool success = true;
		LOG("Loading %s model", file_name.c_str())
		success = LoadTextures(scene);
		if(success)
			success = LoadMeshes(scene);
		if(success)
			loaded = true;
		else {
			LOG("Could not load model %s", file_name.c_str())
			CleanUp();
		}			
	}
	else
	{
		LOG("Error loading file %s: %s", file_name.c_str(), aiGetErrorString());
	}
}

bool Model::LoadTextures(const aiScene* scene)
{
	aiString file;
	textures.reserve(scene->mNumMaterials);
	LOG("Loading %d textures", scene->mNumMaterials)
	for (unsigned i = 0; i < scene->mNumMaterials; i++)
	{
		// Atm we only support loading a single texture so index is hardcoded to 0
		static const int index = 0;
		auto a = scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, index, &file);
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
			{
				LOG("Failed to load texture from model file path: %s", texture.path.c_str())
				texture = App->textures->Load((default_path + texture_file).c_str());
			}				
			if (!texture.loaded) {
				LOG("Failed to load texture from default: %s", texture.path.c_str())
				LOG("Could not find texture %s")
				return false;
			}				
			textures.push_back(texture);
			LOG("Loaded texture from %s", texture.path.c_str())
		}
		else {
			LOG("Error loading textures %s: %s", file_name.c_str(), aiGetErrorString());
			return false;
		}
	}
	return true;
}

bool Model::LoadMeshes(const aiScene* scene)
{
	// Create vbo using mVertices, mTextureCoords and mNumVertices
	textures.reserve(scene->mNumMeshes);
	LOG("Loading %d meshes", scene->mNumMeshes)
	for (unsigned i = 0; i < scene->mNumMeshes; i++)
	{
		LOG("Loaded mesh %d", i);
		Mesh mesh = Mesh();
		mesh.Load(scene->mMeshes[i]);
		if (mesh.IsLoaded()) {
			meshes.push_back(mesh);
		}
		else {
			LOG("Error loading mesh %d: %s", i, aiGetErrorString());
			return false;
		}			
	}
	return true;
}

void Model::CleanUp()
{
	
	for (Mesh& mesh : meshes) {
		mesh.CleanUp();
	}
	meshes.clear();
	for (Texture texture : textures) {
		glDeleteTextures(1, &texture.id);
	}
	textures.clear();
	loaded = false;
	LOG("Model %s unloaded", file_name.c_str())
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
	if (ImGui::SliderFloat3("s.XYZ", &scale[0], 0.005f, 5.0f)) {
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
					matrix.scaleX = scale[i];
					matrix.scaleY = scale[i];
					matrix.scaleZ = scale[i];
					break; // Only one axis can change
				}
			}
		}
	}
	// TODO: Add rotation
	if (ImGui::Button("Unload"))
		CleanUp();
}

void Model::PropertiesWindow(bool* p_open)
{	
	static const ImVec4 yellow(1.0f, 1.0f, 0.0f, 1.0f);
	ImGui::SetNextWindowSize(ImVec2(250, 200), ImGuiCond_Once);
	ImGui::Begin("Model Properties", p_open); // TODO: Fill with filename

	if (!loaded) {
		ImGui::Text("No model loaded");
		ImGui::End();
		return;
	}

	ImGui::TextColored(yellow, "Model");
	ImGui::Text("Name: %s", name.c_str());
	ImGui::Text("Path: %s", (path + file_name).c_str());
	ImGui::Separator();
	ImGui::TextColored(yellow, "Textures");
	for (int i = 0; i < textures.size(); ++i) {
		const Texture& texture = textures[i];
		ImGui::Text("T[%d]: %dx%d %s", i, texture.width, texture.height, texture.path.c_str());
		ImGui::Image((void*)(intptr_t)texture.id, ImVec2(150, 150));
	}

	ImGui::Separator();
	ImGui::TextColored(yellow, "Meshes");
	int vertices = 0;
	int indexes = 0;
	for (int i = 0; i < meshes.size(); ++i) {
		const Mesh& mesh = meshes[i];
		ImGui::Text("M[%d]: %dt, (%dv, %di)", i, mesh.GetNumIndexes()/3, mesh.GetNumVertices(), mesh.GetNumIndexes());
		indexes += mesh.GetNumIndexes();
		vertices += mesh.GetNumVertices();
	}
	ImGui::Text("Total : %dt, (%dv, %di)", indexes/3, vertices, indexes);

	ImGui::Separator();
	ImGui::TextColored(yellow, "Transform");
	for (int r = 0; r < 4; ++r) {
		auto row = matrix.Row(r);
		ImGui::Text("%.2f, %.2f, %.2f, %.2f", row[0], row[1], row[2], row[3]);
	}
	


	ImGui::End();
}
