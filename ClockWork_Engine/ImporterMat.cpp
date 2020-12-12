#include "ImporterMat.h"
#include "Application.h"
#include "Timer.h"
#include "ResourceMat.h"
#include "FileSys.h"
#include "Assimp/include/material.h"

void ImporterMat::Import(const aiMaterial* aimaterial, ResourceMaterial* material)
{
	Timer timer;
	timer.Start();

	aiString path;
	aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path);
	aiColor3D aiDiffuseColor;
	aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiffuseColor);

	if (path.length > 0)
	{
		std::string file_path = material->assetsFile;
		path = FileSys::GetFile(path.C_Str());
		file_path = FindTexture(path.C_Str(), material->assetsFile.c_str());

		if (file_path.size() > 0)
		{
			std::string meta_file = App->res->GenerateMetaFile(file_path.c_str());
			if (!FileSys::Exists(meta_file.c_str()))
				material->diffuseTextureUID = App->res->ImportFile(file_path.c_str());
			else
				material->diffuseTextureUID = App->res->Find(file_path.c_str());
		}

		material->diffuseColor.r = aiDiffuseColor.r;
		material->diffuseColor.g = aiDiffuseColor.g;
		material->diffuseColor.b = aiDiffuseColor.b;

		//LOG("%s imported in %.3f s", texture->path.c_str(), timer.ReadSec());
	}
}



std::string ImporterMat::FindTexture(const char* texture_name, const char* model_directory)
{
	std::string path;
	FileSys::SplitFilePath(model_directory, &path);
	std::string texture_path = path + texture_name;

	//Check if the texture is in the same folder
	if (FileSys::Exists(texture_path.c_str()))
	{
		return texture_path.c_str();
	}
	else
	{
		//Check if the texture is in a sub folder
		texture_path = path + "Textures/" + texture_name;
		if (FileSys::Exists(texture_path.c_str()))
		{
			return texture_path.c_str();
		}
		else
		{
			//Check if the texture is in the root textures folder
			texture_path = std::string("Assets/Textures/") + texture_name;
			if (FileSys::Exists(texture_path.c_str()))
			{
				return texture_path.c_str();
			}
		}
	}

	texture_path.clear();
	return texture_path;
}

