#include "Globals.h"
#include "Model.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"

void Model::Load(const char* file_name)
{
	/*const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene){
		// TODO: LoadTextures(scene->mMaterials, scene->mNumMaterials);
		// TODO: 
	}
	else {
		LOG("Error loading %s: %s", file_name, aiGetErrorString())
	}*/
}

void Model::LoadMaterial(const aiScene* scene)
{
	/*
	aiString file;
	// TODO create materials data structure
	materials.reserve(scene->mNumMaterials);
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			// TODO: Create textures module
			//materials.push_back(App->textures->Load(file.data));
		}
	}*/
}
