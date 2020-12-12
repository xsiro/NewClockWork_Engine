#pragma once
#include "Globals.h"
#include <string>

class GameObject;
class ResourceComp;
enum ResourceType;

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



	bool IsEnabled();
	ComponentType GetType();
	virtual void OnEditor() = 0;

	void SetGameObject(GameObject* gameObject);
	GameObject* GetGameObject();
	virtual void SetResourceUID(uint UID);
	virtual ResourceComp* GetResource(ResourceType type) { return nullptr; };

public:
	std::string name;

protected:
	ComponentType type;
	GameObject* _gameObject;
	bool enabled;

	uint _resourceUID;
};
