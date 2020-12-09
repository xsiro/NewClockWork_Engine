#pragma once
#include "GameObject.h"
#include "ModuleComponent.h"
#include "ModuleTransform.h"
#include "ModuleMesh.h"
#include "ModuleMaterial.h"
#include "imgui.h"

#include <vector>

GameObject::GameObject() : active(true), name("Game Object"), parent(nullptr)
{
	CreateComponent(ComponentType::Transform);

}

GameObject::~GameObject()
{
	parent = nullptr;
	components.clear();
	children.clear();
}

void GameObject::Update() 
{
	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->Update();
	}

	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->Update();
	}
}
bool GameObject::IsEnabled()
{
	return enabled;
}

void GameObject::ChangeName(char* new_name)
{
	name = new_name;
}

ModuleComponent* GameObject::GetComponent(ComponentType component) {

	for (size_t i = 0; i < components.size(); i++)
	{
		if (components[i]->ReturnType() == component)
		{
			return components[i];
		}
	}

	return nullptr;
}

ModuleComponent* GameObject::CreateComponent(ComponentType type) {

	ModuleComponent* component = nullptr;
	switch (type)
	{
	case ComponentType::Transform:
		component = new ModuleTransform();
		components.push_back(component);
		break;
	case ComponentType::Mesh:
		component = new ModuleMesh();
		components.push_back(component);
		break;
	case ComponentType::Material:
		component = new ModuleMaterial();
		components.push_back(component);
		break;
	}
	return component;
}

bool GameObject::DeleteComponent(ModuleComponent* component)
{
	for (size_t i = 0; i < components.size(); i++)
	{
		if (components[i] == component) {
			delete components[i];
			components.erase(components.begin() + i);
			component = nullptr;
			return true;
		}
	}

	return false;
}



