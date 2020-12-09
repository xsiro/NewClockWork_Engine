#pragma once
#include "Globals.h"
#include <vector>
#include <iostream>
#include <string>
#include "imgui.h"

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
	void Enable(); //Enable GameObject
	void Disable(); //Disable GameObject
	bool IsEnabled();
	void ChangeName(char* new_name);

public:
	GameObject* parent;
	bool enabled;
	bool active;
	int id;
	bool selected;
	std::string name;
	std::vector<ModuleComponent*> components;
	std::vector<GameObject*> children;
};