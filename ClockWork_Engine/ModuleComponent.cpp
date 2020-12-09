#pragma once
#include "ModuleComponent.h"

ModuleComponent::ModuleComponent()
{
}

ModuleComponent::~ModuleComponent()
{

}

void ModuleComponent::Enable() {

	active = true;

}

void ModuleComponent::Update() {

}

void ModuleComponent::Disable() {

	active = false;
}

ComponentType ModuleComponent::ReturnType() {

	return type;

}

GameObject* ModuleComponent::ReturnGameObject() {

	return owner;

}
