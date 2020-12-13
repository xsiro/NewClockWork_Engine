#pragma once

#include "Globals.h"

#include <vector>
#include <string>

#include "MathGeoLib/include/MathGeoLib.h"

class ModuleComponent;
class ModuleTransform;
class GnMesh;
enum ComponentType;
class JSON;
class GnJSONArray;


class GameObject {
public:
	GameObject();
	GameObject(GnMesh* mesh);
	~GameObject();

	void Update();
	void OnEditor();

	ModuleComponent* GetComponent(ComponentType component);
	std::vector<ModuleComponent*> GetComponents();
	ModuleComponent* AddComponent(ComponentType type);
	void AddComponent(ModuleComponent* component);
	bool RemoveComponent(ModuleComponent* component);

	const char* GetName();
	void SetName(const char* name);
	void SetTransform(ModuleTransform transform);
	ModuleTransform* GetTransform();
	AABB GetAABB();
	bool IsVisible();
	void Save(GnJSONArray& save_array);
	uint Load(JSON* object);
	uint LoadNodeData(JSON* object);
	GameObject* GetParent();
	void SetParent(GameObject* parent);
	void Reparent(GameObject* newParent);

	void AddChild(GameObject* child);
	int GetChildrenAmount();
	GameObject* GetChildAt(int index);
	bool RemoveChild(GameObject* gameObject);
	void DeleteChildren();
	void UpdateChildrenTransforms();

public:
	bool to_delete;
	uint UUID = 0;

private:
	void GenerateAABB(GnMesh* mesh);

private:
	bool enabled;
	bool _visible;
	std::string name;
	GameObject* _parent;
	ModuleTransform* transform;
	std::vector<ModuleComponent*> components;
	std::vector<GameObject*> children;

	OBB _OBB;
	AABB _AABB;
};
