#pragma once
#include "Globals.h"
#include "JSON.h"
#include "Resource.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <vector>

class Light;
class Camera;

struct ModelNode
{
	std::string name;
	float3 position;
	Quat rotation;
	float3 scale;

	int meshID = -1;
	int materialID = -1;

	uint UID;
	uint parentUID;
};

class ResourceMod : public Resource {
public:
	ResourceMod(uint UID);
	~ResourceMod();
	uint Save(JSON& base_object) override;
	uint SaveMeta(JSON& base_object, uint last_modification) override;

public:
	std::vector<ModelNode> nodes;
	std::vector<uint> meshes;
	std::vector<uint> materials;
	std::vector<uint> textures;
	std::vector<Light*> lights;
	std::vector<Camera*> cameras;
};