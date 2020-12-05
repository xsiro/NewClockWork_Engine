#pragma once
#include "Globals.h"
#include <vector>
#include <iostream>
#include <string>

class ModuleComponent;
enum class ComponentType;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update();

	ModuleComponent* GetComponent(ComponentType component);
	ModuleComponent* CreateComponent(ComponentType type);
	bool DeleteComponent(ModuleComponent* component);
	void AddComponent(ModuleComponent* component);

public:
	GameObject* parent;
	bool active;
	std::string name;
	std::vector<ModuleComponent*> components;
	std::vector<GameObject*> children;
};