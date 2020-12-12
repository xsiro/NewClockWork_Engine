#include "ModuleComponent.h"

ModuleComponent::ModuleComponent() : enabled(true), _gameObject(nullptr) {}

ModuleComponent::ModuleComponent(GameObject* gameObject)
{
    _gameObject = gameObject;
}

ModuleComponent::~ModuleComponent()
{
    _gameObject = nullptr;
}

void ModuleComponent::Update() {}

void ModuleComponent::Enable() { enabled = true; }

void ModuleComponent::Disable() { enabled = false; }

bool ModuleComponent::IsEnabled()
{
    return enabled;
}

ComponentType ModuleComponent::GetType()
{
    return type;
}

void ModuleComponent::SetGameObject(GameObject* g_gameObject)
{
    _gameObject = g_gameObject;
}

GameObject* ModuleComponent::GetGameObject()
{
    return _gameObject;
}

void ModuleComponent::SetResourceUID(uint UID)
{
    _resourceUID = UID;
}
