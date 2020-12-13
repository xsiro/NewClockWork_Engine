#pragma once

#include "Globals.h"

#include <string>
class JSON;

enum ResourceType
{
	RESOURCE_MODEL,
	RESOURCE_MESH,
	RESOURCE_MATERIAL,
	RESOURCE_TEXTURE,
	RESOURCE_SCENE,
	RESOURCE_UNKNOWN
};

class Resource
{
public:
	Resource(uint uid, ResourceType type);
	virtual ~Resource();

	ResourceType GetType();
	uint GetUID();
	virtual uint Save(JSON& base_object) { return -1; };
	virtual uint SaveMeta(JSON& base_object, uint last_modification) { return -1; };
	virtual void Load(JSON& base_object) {};

public:
	std::string name;
	std::string assetsFile;
	std::string libraryFile;

	int referenceCount = 0;

protected:
	uint _uid = 0;
	ResourceType _type = RESOURCE_UNKNOWN;
};
