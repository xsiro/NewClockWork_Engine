#include "ResourceMod.h"
#include "Application.h"
//#include "Importers.h"
//#include "Camera.h"
#include "Light.h"

ResourceMod::ResourceMod(uint UID) : Resource(UID, ResourceType::RESOURCE_MODEL) {}

ResourceMod::~ResourceMod()
{
	nodes.clear();
	meshes.clear();
	materials.clear();
	textures.clear();

	for (size_t i = 0; i < lights.size(); i++)
	{
		lights[i] = nullptr;
	}
	lights.clear();

	for (size_t i = 0; i < cameras.size(); i++)
	{
		cameras[i] = nullptr;
	}
	cameras.clear();
}

