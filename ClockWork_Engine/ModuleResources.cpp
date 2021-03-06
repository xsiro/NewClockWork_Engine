#include "ModuleResources.h"
#include "GameObject.h"
#include "FileSys.h"
#include "Importer.h"
#include "ModuleSceneIntro.h"
#include "Application.h"
#include "JSON.h"
#include "Resource.h"
#include "ResourceMod.h"
#include "ResourceMesh.h"
#include "ResourceMat.h"
#include "ResourceTex.h"

#include "Import.h"
#include "Assets.h"
#include <algorithm>

#include "MathGeoLib/include/MathGeoLib.h"

ModuleResources::ModuleResources(bool start_enabled) : Module(start_enabled), _toDeleteAsset(-1), _toDeleteResource(-1)
{
	name = "resources";
	modelImportingOptions = Options();
	textureImportingOptions = TextureImportingOptions();
}

ModuleResources::~ModuleResources() {}

bool ModuleResources::Init()
{
	bool ret = true;

	ImporterMesh::Init();
	ImporterTex::Init();

	//std::vector<std::string> files;
	//std::vector<std::string> dirs;
	//FileSystem::DiscoverFilesRecursive("Library", files, dirs);

	CheckAssetsRecursive("Assets");

	//LoadEngineAssets();

	return ret;
}

bool ModuleResources::CleanUp()
{
	bool ret = true;

	for (std::map<uint, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		ReleaseResource(it->second->GetUID());
	}

	resources.clear();
	resources_data.clear();

	return ret;
}

void ModuleResources::OnEditor()
{
	std::vector<Resource*> meshes;
	std::vector<Resource*> materials;
	std::vector<Resource*> textures;

	std::map<uint, Resource*>::iterator it = resources.begin();
	for (it; it != resources.end(); it++)
	{
		switch (it->second->GetType())
		{
		case ResourceType::RESOURCE_MESH:
			meshes.push_back(it->second);
			break;
		case ResourceType::RESOURCE_MATERIAL:
			materials.push_back(it->second);
			break;
		case ResourceType::RESOURCE_TEXTURE:
			textures.push_back(it->second);
			break;
		default:
			break;
		}
	}

	if (ImGui::TreeNode("Meshes")) {
		ImGui::Separator();
		for (size_t i = 0; i < meshes.size(); i++)
		{
			ImGui::Text("Name: %s", meshes[i]->name.c_str());
			ImGui::Text("UID: %d", meshes[i]->GetUID());
			ImGui::Text("Assets path: %s", meshes[i]->assetsFile.c_str());
			ImGui::Text("Library path: %s", meshes[i]->libraryFile.c_str());
			ImGui::Text("Reference count: %d", meshes[i]->referenceCount);
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Materials")) {
		for (size_t i = 0; i < materials.size(); i++)
		{
			ImGui::Text("Name: %s", materials[i]->name.c_str());
			ImGui::Text("UID: %d", materials[i]->GetUID());
			ImGui::Text("Assets path: %s", materials[i]->assetsFile.c_str());
			ImGui::Text("Library path: %s", materials[i]->libraryFile.c_str());
			ImGui::Text("Reference count: %d", materials[i]->referenceCount);
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Textures")) {
		for (size_t i = 0; i < textures.size(); i++)
		{
			ImGui::Text("Name: %s", textures[i]->name.c_str());
			ImGui::Text("UID: %d", textures[i]->GetUID());
			ImGui::Text("Assets path: %s", textures[i]->assetsFile.c_str());
			ImGui::Text("Library path: %s", textures[i]->libraryFile.c_str());
			ImGui::Text("Reference count: %d", textures[i]->referenceCount);
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
		}
		ImGui::TreePop();
	}
}

void ModuleResources::LoadEngineAssets()
{
	ResourceTex* folder_tex = dynamic_cast<ResourceTex*>(RequestResource(Find("Assets/Folder.png")));
}

void ModuleResources::OnFrameEnd()
{
	if (_toDeleteResource != -1)
	{
		DeleteResource(_toDeleteResource);
		_toDeleteResource = -1;
	}

	if (_toDeleteAsset != -1)
	{
		std::string assets_path = resources_data[_toDeleteAsset].assetsFile;
		DeleteResource(_toDeleteAsset);
		DeleteAsset(assets_path.c_str());
		_toDeleteAsset = -1;
	}
}


bool ModuleResources::MetaUpToDate(const char* assets_file, const char* meta_file)
{
	bool ret = true;

	char* buffer = nullptr;
	uint size = FileSys::Load(meta_file, &buffer);
	JSON meta(buffer);

	uint UID = meta.GetInt("UID");
	int lastModifiedMeta = meta.GetInt("lastModified");
	uint lastModified = FileSys::GetLastModTime(assets_file);

	if (lastModifiedMeta == lastModified && UID != 0)
	{
		std::string library_path = meta.GetString("Library path", "Library/NoPath");

		if (library_path == "Library/NoPath")
			library_path = Find(UID);

		//check for the file itself to exist
		if (!FileSys::Exists(library_path.c_str()))
		{
			ret = false;
		}
		//check its internal resources
		else if (GetTypeFromPath(assets_file) == ResourceType::RESOURCE_MODEL)
		{
			ret = ImporterMod::InternalResourcesExist(meta_file);
		}

		resources_data[UID].assetsFile = assets_file;
		resources_data[UID].libraryFile = library_path;
		resources_data[UID].type = GetTypeFromPath(assets_file);
	}
	else
	{
		ret = false;
	}

	meta.Release();
	RELEASE_ARRAY(buffer);

	return ret;
}

int ModuleResources::GetUIDFromMeta(const char* meta_file)
{
	char* buffer = nullptr;
	uint size = FileSys::Load(meta_file, &buffer);
	JSON meta(buffer);

	int UID = meta.GetInt("UID", -1);

	meta.Release();
	RELEASE_ARRAY(buffer);

	return UID;
}

int ModuleResources::Find(const char* assets_file)
{
	int UID = -1;

	std::map<uint, Resource*>::iterator resource_it = resources.begin();
	std::map<uint, ResourceData>::iterator resources_data_it = resources_data.begin();

	//First we loop through all loaded resources
	for (resource_it; resource_it != resources.end(); resource_it++) {
		if (FileSys::ToLower(resource_it->second->assetsFile.c_str()) == FileSys::ToLower(assets_file))
			return resource_it->first;
	}

	//If not found we loop through all not loaded but known resources
	for (resources_data_it; resources_data_it != resources_data.end(); resources_data_it++) {
		if (FileSys::ToLower(resources_data_it->second.assetsFile.c_str()) == FileSys::ToLower(assets_file))
			return resources_data_it->first;
	}

	return UID;
}

const char* ModuleResources::Find(uint UID)
{
	if (resources_data.find(UID) != resources_data.end() && resources_data[UID].libraryFile.size() > 0)
		return resources_data[UID].libraryFile.c_str();

	std::vector<std::string> directories = { "Assets/config/", "Assets/Models", "Assets/Textures/","Assets/Scenes/" };
	std::vector<std::string> extensions = { ".json",".model",".mesh",".material",".dds", ".scene" };

	for (size_t i = 0; i < directories.size(); i++)
	{
		std::string file = directories[i];
		file += std::to_string(UID);
		file += extensions[i];
		if (FileSys::Exists(file.c_str()))
		{
			char* final_file = new char[256];
			sprintf_s(final_file, 256, file.c_str());
			std::string library_path = final_file;
			resources_data[UID].libraryFile = library_path;
			return final_file;
		}
	}

	return nullptr;
}

const char* ModuleResources::GetLibraryPath(uint UID)
{
	std::map<uint, Resource*>::iterator resources_it = resources.find(UID);
	std::map<uint, ResourceData>::iterator resources_data_it = resources_data.find(UID);

	if (resources_it != resources.end()) {
		return resources_it->second->libraryFile.c_str();
	}

	if (resources_data_it != resources_data.end()) {
		return resources_data_it->second.libraryFile.c_str();
	}
}

bool ModuleResources::Exists(uint UID)
{
	std::map<uint, Resource*>::iterator it = resources.find(UID);

	if (it != resources.end())
		return true;
	else
		return false;
}



uint ModuleResources::ImportFile(const char* assets_file)
{

	ResourceType type = GetTypeFromPath(assets_file);

	Resource* resource = CreateResource(assets_file, type);
	uint ret = 0;

	char* fileBuffer;
	uint size = FileSys::Load(assets_file, &fileBuffer);
	std::string library_path;
	switch (type)
	{
	case RESOURCE_MODEL:
		ImporterMod::Import(fileBuffer, (ResourceMod*)resource, size);
		break;
	case RESOURCE_TEXTURE:
		ImporterTex::Import(fileBuffer, (ResourceTex*)resource, size);
		break;
	case RESOURCE_SCENE:
		library_path = "/Scenes";
		library_path.append(FileSys::GetFile(assets_file));
		FileSys::DuplicateFile(assets_file, library_path.c_str());
		library_path.clear();
		break;
	default:
		LOG_WARNING("Trying to import unknown file: %s", assets_file);
		break;
	}

	if (resource == nullptr) {
		LOG_ERROR("Fatal error when importing file: %s", assets_file);
		return 0;
	}

	SaveResource(resource);
	ret = resource->GetUID();
	ReleaseResource(ret);
	RELEASE_ARRAY(fileBuffer);

	return ret;
}

uint ModuleResources::ImportInternalResource(const char* path, const void* data, ResourceType type, uint UID)
{
	uint ret = 0;
	Resource* resource = CreateResource(path, type, UID);

	switch (type)
	{
	case RESOURCE_MESH:
		ImporterMesh::Import((aiMesh*)data, (ResourceMesh*)resource);
		break;
	case RESOURCE_MATERIAL:
		ImporterMat::Import((aiMaterial*)data, (ResourceMat*)resource);
		break;
	default:
		break;
	}

	SaveResource(resource);
	ret = resource->GetUID();
	ReleaseResource(ret);

	return ret;
}

uint ModuleResources::ReimportFile(const char* assets_file)
{
	LOG_WARNING("Reimporting file: %s", assets_file);

	std::string meta_file = assets_file;
	meta_file.append(".meta");

	uint ret = GetUIDFromMeta(meta_file.c_str());
	ResourceType type = GetTypeFromPath(assets_file);
	Resource* resource = CreateResource(assets_file, type, ret);

	char* fileBuffer;
	uint size = FileSys::Load(assets_file, &fileBuffer);

	if (size <= 0)
	{
		LOG_ERROR("Trying to load unexisting file: %s", assets_file);
		return 0;
	}

	switch (type)
	{
	case ResourceType::RESOURCE_MODEL:
		ImporterMod::ReimportFile(fileBuffer, (ResourceMod*)resource, size);
		break;
	case ResourceType::RESOURCE_TEXTURE:
		if (FileSys::Exists(resource->libraryFile.c_str()))
			FileSys::Delete(resource->libraryFile.c_str());

		ImporterTex::Import(fileBuffer, (ResourceTex*)resource, size);
		break;
	default:
		break;
	}

	if (resource == nullptr)
	{
		LOG_ERROR("Fatal error when reimporting file: %s", assets_file);
		return 0;
	}
	else
	{
		LOG("File: %s reimported successfully", assets_file);
	}

	SaveResource(resource);
	ret = resource->GetUID();
	ReleaseResource(ret);
	RELEASE_ARRAY(fileBuffer);

	return ret;
}

void ModuleResources::CreateResourceData(uint UID, const char* name, const char* assets_path, const char* library_path)
{
	resources_data[UID].name = name;
	resources_data[UID].assetsFile = assets_path;
	resources_data[UID].libraryFile = library_path;
	resources_data[UID].type = GetTypeFromPath(library_path);
}

void ModuleResources::DragDropFile(const char* path)
{
	std::string file_to_import = path;

	if (!FileSys::Exists(path))
	{
		file_to_import = GenerateAssetsPath(path);
		FileSys::DuplicateFile(path, file_to_import.c_str());
	}

	char* final_path = new char[sizeof(char) * file_to_import.size()];
	strcpy(final_path, file_to_import.c_str());
	Import* import_window = dynamic_cast<Import*>(App->gui->windows[WindowType::IMPORT_WINDOW]);
	import_window->Enable(final_path, GetTypeFromPath(path));
}

void ModuleResources::AddAssetToDelete(const char* asset_path)
{
	std::string meta_file = asset_path;
	meta_file.append(".meta");
	_toDeleteAsset = GetUIDFromMeta(meta_file.c_str());
	App->AddModuleToTaskStack(this);
}

void ModuleResources::AddResourceToDelete(uint UID)
{
	_toDeleteResource = UID;
	App->AddModuleToTaskStack(this);
}

bool ModuleResources::DeleteAsset(const char* assets_path)
{
	bool ret = true;

	FileSys::Delete(assets_path);

	std::string meta_file = assets_path;
	meta_file.append(".meta");

	if (FileSys::Exists(meta_file.c_str()))
		FileSys::Delete(meta_file.c_str());

	return ret;
}

bool ModuleResources::DeleteResource(uint UID)
{
	bool ret = true;

	switch (resources_data[UID].type)
	{
	case ResourceType::RESOURCE_MODEL:
		DeleteInternalResources(UID);
		break;
	case ResourceType::RESOURCE_TEXTURE:
		break;
	default:
		break;
	}

	ReleaseResource(UID);
	FileSys::Delete(resources_data[UID].libraryFile.c_str());
	ReleaseResourceData(UID);

	return ret;
}

bool ModuleResources::DeleteInternalResources(uint UID)
{
	bool ret = true;
	std::string meta_file = resources_data[UID].assetsFile + ".meta";

	if (resources_data[UID].type == ResourceType::RESOURCE_MODEL)
	{
		std::vector<uint> meshes;
		std::vector<uint> materials;
		ImporterMod::ExtractInternalResources(resources_data[UID].libraryFile.c_str(), meshes, materials);

		for (size_t i = 0; i < meshes.size(); i++)
		{
			DeleteInternalResource(meshes[i]);
		}

		for (size_t i = 0; i < materials.size(); i++)
		{
			DeleteInternalResource(materials[i]);
		}
	}

	return ret;
}

bool ModuleResources::DeleteInternalResource(uint UID)
{
	bool ret = true;

	std::string library_path = Find(UID);
	ret = FileSys::Delete(library_path.c_str());

	ReleaseResource(UID);

	return ret;
}

Resource* ModuleResources::LoadResource(uint UID, ResourceType type)
{
	Resource* resource = CreateResource(UID, type, resources_data[UID].assetsFile.c_str());
	bool ret = true;

	char* buffer = nullptr;
	uint size = FileSys::Load(resource->libraryFile.c_str(), &buffer);

	if (size >= 0)
	{
		switch (resource->GetType())
		{
		case RESOURCE_MODEL:
			if (ImporterMod::InternalResourcesExist(resource->libraryFile.c_str()))
				ret = ImporterMod::Load(buffer, (ResourceMod*)resource, size);
			else {
				ReimportFile(resources_data[UID].assetsFile.c_str());
				ret = ImporterMod::Load(buffer, (ResourceMod*)resource, size);
			}
			break;
		case RESOURCE_MESH:
			ret = ImporterMesh::Load(buffer, (ResourceMesh*)resource, size);
			break;
		case RESOURCE_MATERIAL:
			ret = ImporterMat::Load(buffer, (ResourceMat*)resource, size);
			break;
		case RESOURCE_TEXTURE:
			/*ret = ImporterTex::Load(buffer, (ResourceTex*)resource, size);*/
			/*LoadMetaFile(resource);*/
			break;
		case RESOURCE_SCENE:
			break;
		case RESOURCE_UNKNOWN:
			break;
		default:
			break;
		}
	}
	else
		ret = false;

	if (ret == false)
	{
		LOG_ERROR("Resource: %d could not be loaded");
		ReleaseResource(UID);
		resource = nullptr;
		return nullptr;
	}

	RELEASE_ARRAY(buffer);

	return resource;
}

void ModuleResources::UnloadResource(Resource* resource)
{
	switch (resource->GetType())
	{
	case ResourceType::RESOURCE_MODEL:
		break;
	case ResourceType::RESOURCE_MESH:
		break;
	case ResourceType::RESOURCE_MATERIAL:
		break;
	case ResourceType::RESOURCE_TEXTURE:
		ImporterTex::UnloadTexture(((ResourceTex*)resource)->GetID());
		break;
	default:
		break;
	}

	resources.erase(resources.find(resource->GetUID()));

	delete resource;
	resource = nullptr;
}

Resource* ModuleResources::CreateResource(const char* assetsPath, ResourceType type, uint UID)
{
	Resource* resource = nullptr;

	if (UID == 0)
		UID = GenerateUID();

	switch (type)
	{
	case RESOURCE_MODEL:
		resource = new ResourceMod(UID);
		break;
	case RESOURCE_MESH:
		resource = new ResourceMesh(UID);
		break;
	case RESOURCE_MATERIAL:
		resource = new ResourceMat(UID);
		break;
	case RESOURCE_TEXTURE:
		resource = new ResourceTex(UID);
		break;
	case RESOURCE_SCENE:
		break;
	case RESOURCE_UNKNOWN:
		break;
	default:
		break;
	}

	if (resource != nullptr)
	{
		resources[UID] = resource;
		resources[UID]->name = FileSys::GetFileName(assetsPath);
		resource->assetsFile = FileSys::ToLower(assetsPath);
		resource->libraryFile = GenerateLibraryPath(resource);

		resources_data[UID].name = resources[UID]->name;
		resources_data[UID].assetsFile = resource->assetsFile;
		resources_data[UID].libraryFile = resource->libraryFile;
		resources_data[UID].type = type;
	}

	return resource;
}

Resource* ModuleResources::CreateResource(uint UID, ResourceType type, std::string assets_file)
{
	Resource* resource = nullptr;

	switch (type)
	{
	case RESOURCE_MODEL:
		resource = new ResourceMod(UID);
		break;
	case RESOURCE_MESH:
		resource = new ResourceMesh(UID);
		break;
	case RESOURCE_MATERIAL:
		resource = new ResourceMat(UID);
		break;
	case RESOURCE_TEXTURE:
		resource = new ResourceTex(UID);
		break;
	case RESOURCE_SCENE:
		break;
	case RESOURCE_UNKNOWN:
		break;
	default:
		break;
	}

	if (resource != nullptr)
	{
		if (assets_file.size() > 0)
			resource->assetsFile = assets_file;
		else
			resource->assetsFile = resources_data[UID].assetsFile;

		resource->libraryFile = GenerateLibraryPath(resource);
		resource->name = FileSys::GetFileName(resource->assetsFile.c_str());

		resources[UID] = resource;
	}

	return resource;
}

Resource* ModuleResources::RequestResource(uint UID)
{
	std::map<uint, Resource*>::iterator it = resources.find(UID);

	if (it != resources.end() && it->second != nullptr) {
		it->second->referenceCount++;
		return it->second;
	}

	const char* library_file = Find(UID);

	if (library_file == nullptr)
		return nullptr;

	ResourceType type = GetTypeFromPath(library_file);
	Resource* resource = LoadResource(UID, type);

	if (resource != nullptr)
		resource->referenceCount++;

	return resource;
}

ResourceData ModuleResources::RequestResourceData(uint UID)
{
	std::map<uint, ResourceData>::iterator it = resources_data.find(UID);

	if (it != resources_data.end())
		return it->second;
}

GameObject* ModuleResources::RequestGameObject(const char* assets_file)
{
	std::string meta_file = assets_file;
	meta_file.append(".meta");
	ResourceMod* model = (ResourceMod*)RequestResource(GetUIDFromMeta(meta_file.c_str()));

	if (model == nullptr)
	{
		model = (ResourceMod*)RequestResource(ReimportFile(assets_file));
	}

	return ImporterMod::ConvertToGameObject(model);
}

void ModuleResources::ReleaseResource(uint UID)
{
	std::map<uint, Resource*>::iterator it = resources.find(UID);
	if (it != resources.end())
	{
		it->second->referenceCount--;

		if (it->second->referenceCount <= 0)
			UnloadResource(it->second);
	}
}

void ModuleResources::ReleaseResourceData(uint UID)
{
	resources_data.erase(resources_data.find(UID));
}

bool ModuleResources::SaveResource(Resource* resource)
{
	bool ret = true;

	char* buffer;
	uint size = 0;

	switch (resource->GetType())
	{
	case RESOURCE_MODEL:
		size = ImporterMod::Save((ResourceMod*)resource, &buffer);
		break;
	case RESOURCE_MESH:
		size = ImporterMesh::Save((ResourceMesh*)resource, &buffer);
		break;
	case RESOURCE_MATERIAL:
		size = ImporterMat::Save((ResourceMat*)resource, &buffer);
		break;
	case RESOURCE_TEXTURE:
		//size = ImporterTex::Save((ResourceTex*)resource, &buffer);
		break;
	case RESOURCE_SCENE:
		break;
	default:
		break;
	}

	if (size > 0)
	{
		FileSys::Save(resource->libraryFile.c_str(), buffer, size);
		RELEASE_ARRAY(buffer);
	}

	//if (resource->GetType() == ResourceType::RESOURCE_MODEL || resource->GetType() == ResourceType::RESOURCE_TEXTURE)
	//	ret = SaveMetaFile(resource);

	return ret;
}


ResourceType ModuleResources::GetTypeFromPath(const char* path)
{
	std::string extension = FileSys::GetFileFormat(path);

	if (extension == ".fbx" || extension == ".model")
		return ResourceType::RESOURCE_MODEL;

	else if (extension == ".mesh")
		return ResourceType::RESOURCE_MESH;

	else if (extension == ".material")
		return ResourceType::RESOURCE_MATERIAL;

	else if (extension == ".png" || extension == ".tga" || extension == ".dds")
		return ResourceType::RESOURCE_TEXTURE;

	else
		return ResourceType::RESOURCE_UNKNOWN;
}

uint ModuleResources::GenerateUID()
{
	return LCG().Int();
}

const char* ModuleResources::GenerateLibraryPath(Resource* resource)
{
	char* library_path = new char[128];

	switch (resource->GetType())
	{
	case RESOURCE_MODEL:
		sprintf_s(library_path, 128, "Assets/%d.model", resource->GetUID()); break;
	case RESOURCE_MESH:
		sprintf_s(library_path, 128, "Assets/%d.mesh", resource->GetUID()); break;
	case RESOURCE_MATERIAL:
		sprintf_s(library_path, 128, "Assets/%d.material", resource->GetUID()); break;
	case RESOURCE_TEXTURE:
		sprintf_s(library_path, 128, "Assets/%d.dds", resource->GetUID()); break;
	case RESOURCE_SCENE:
		sprintf_s(library_path, 128, "Assets/%d.scene", resource->GetUID()); break;
	default:
		break;
	}

	return library_path;
}

std::string ModuleResources::GenerateLibraryPath(uint uid, ResourceType type)
{
	std::string path;
	switch (type)
	{
	case RESOURCE_MODEL:
		path = "";
		path.append(std::to_string(uid) + ".model");
		break;
	case RESOURCE_MESH:
		path = "";
		path.append(std::to_string(uid) + ".mesh");
		break;
	case RESOURCE_MATERIAL:
		path = "";
		path.append(std::to_string(uid) + ".material");
		break;
	case RESOURCE_TEXTURE:
		path = "";
		path.append(std::to_string(uid) + ".dds");
		break;
	case RESOURCE_SCENE:
		path = "";
		path.append(std::to_string(uid) + ".scene");
		break;
	case RESOURCE_UNKNOWN:
		LOG("Error trying to generate a path for an unknown file %d", uid);
		break;
	default:
		break;
	}

	return path;
}

std::string ModuleResources::GetLibraryFolder(const char* file_in_assets)
{
	ResourceType type = GetTypeFromPath(file_in_assets);

	switch (type)
	{
	case RESOURCE_MODEL: return std::string(""); break;
	case RESOURCE_MESH: return std::string(""); break;
	case RESOURCE_MATERIAL: return std::string(""); break;
	case RESOURCE_TEXTURE: return std::string("");	break;
	case RESOURCE_SCENE: return std::string("");	break;
	default: break;
	}
}

std::string ModuleResources::GenerateAssetsPath(const char* path)
{
	ResourceType type = GetTypeFromPath(path);
	std::string file = FileSys::GetFile(path);
	file = FileSys::GetFile(file.c_str());
	std::string assets_path;

	switch (type)
	{
	case RESOURCE_MODEL:
		assets_path = "Assets/Models/"; break;
	case RESOURCE_TEXTURE:
		assets_path = "Assets/Textures/"; break;
	case RESOURCE_SCENE:
		assets_path = "Assets/Scenes/"; break;
	default:
		break;
	}

	return assets_path;
}

std::string ModuleResources::GenerateMetaFile(const char* assets_path)
{
	std::string meta_file = assets_path;
	meta_file.append(".meta");
	return meta_file;
}

void ModuleResources::AddFileExtension(std::string& file, ResourceType type)
{
	switch (type)
	{
	case RESOURCE_MODEL: file += ".model"; break;
	case RESOURCE_MESH: file += ".mesh"; break;
	case RESOURCE_MATERIAL: file += ".material"; break;
	case RESOURCE_TEXTURE: file += ".dds"; break;
	case RESOURCE_SCENE:  file += ".scene";	break;
	default: break;
	}
}

void ModuleResources::CheckAssetsRecursive(const char* directory)
{
	std::vector<std::string> files;
	std::vector<std::string> dirs;

	std::string dir((directory) ? directory : "");
	dir += "/";

	FileSys::DiscoverFiles(dir.c_str(), files, dirs);

	for (std::vector<std::string>::const_iterator it = dirs.begin(); it != dirs.end(); ++it)
	{
		CheckAssetsRecursive((dir + (*it)).c_str());
	}

	std::sort(files.begin(), files.end());

	for (size_t i = 0; i < files.size(); i++)
	{
		std::string file = directory;
		file.append("/" + files[i]);

		//ignore metas and jsons
		if ((file.find(".meta") != std::string::npos) || (file.find(".json") != std::string::npos))
			continue;

		std::string meta = file;
		meta.append(".meta");

		if (FileSys::Exists(meta.c_str()))
		{
			if (!MetaUpToDate(file.c_str(), meta.c_str()))
			{
				ReimportFile(file.c_str());
			}
		}
		else
		{
			ImportFile(file.c_str());
		}
	}
}
