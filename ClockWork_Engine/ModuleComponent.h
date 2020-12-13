#pragma once
#include "Globals.h"
#include <string>

class GameObject;
class Resource;
enum ResourceType;
class JSON;
class GnJSONArray;

enum ComponentType {
	TRANSFORM,
	MESH,
	MATERIAL,
	CAMERA,
	LIGHT
};

class ModuleComponent {
public:
	ModuleComponent();
	ModuleComponent(GameObject* gameObject);
	virtual ~ModuleComponent();
	virtual void Update();
	virtual void Enable();
	virtual void Disable();
	virtual void Save(GnJSONArray& save_array) {};
	virtual void Load(JSON& load_object) {};


	bool IsEnabled();
	ComponentType GetType();
	virtual void OnEditor() = 0;

	void SetGameObject(GameObject* gameObject);
	GameObject* GetGameObject();
	virtual void SetResourceUID(uint UID);
	virtual Resource* GetResource(ResourceType type) { return nullptr; };

public:
	std::string name;

protected:
	ComponentType type;
	GameObject* _gameObject;
	bool enabled;

	uint _resourceUID;
};
