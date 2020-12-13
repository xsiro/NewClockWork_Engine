#include "GameObject.h"
#include "ModuleComponent.h"
#include "ModuleTransform.h"
#include "ModuleMesh.h"
#include "ModuleMaterial.h"
#include "imgui.h"
#include "Application.h"
#include "Cam.h"

#include "MathGeoLib/include/MathGeoLib.h"

#include <vector>

GameObject::GameObject() : enabled(true), name("Game Object"), _parent(nullptr), to_delete(false), transform(nullptr), _visible(false)
{
	transform = (ModuleTransform*)AddComponent(TRANSFORM);
	UUID = LCG().Int();
}

GameObject::GameObject(GnMesh* mesh) : GameObject()
{
	SetName(mesh->name);
	AddComponent((ModuleComponent*)mesh);
}

GameObject::~GameObject()
{
	_parent = nullptr;

	for (size_t i = 0; i < components.size(); i++)
	{
		delete components[i];
		components[i] = nullptr;
	}

	transform = nullptr;
	components.clear();
	children.clear();
	name.clear();
	UUID = 0;
}

void GameObject::Update()
{
	if (enabled)
	{
		for (size_t i = 0; i < components.size(); i++)
		{
			//Update Components
			if (components[i]->IsEnabled())
			{
				if (components[i]->GetType() == ComponentType::MESH)
				{
					GnMesh* mesh = (GnMesh*)components[i];
					GenerateAABB(mesh);

					if (App->renderer3D->IsInsideCameraView(_AABB))
						mesh->Update();
				}
				else
				{
					components[i]->Update();
				}
			}
		}

		//Update Children
		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->Update();
		}
	}
}

void GameObject::OnEditor()
{
	ImGui::Checkbox("Enabled", &enabled);
	ImGui::SameLine();

	static char buf[64] = "Name";
	strcpy(buf, name.c_str());
	if (ImGui::InputText("", &buf[0], IM_ARRAYSIZE(buf))) {}

	for (size_t i = 0; i < components.size(); i++)
	{
		components[i]->OnEditor();
	}

	if (ImGui::CollapsingHeader("Debugging Information"))
	{
		if (_parent != nullptr)
			ImGui::Text("Parent: %s", _parent->GetName());
		else
			ImGui::Text("No parent");

		ImGui::Text("UUID: %d", UUID);
	}
}



ModuleComponent* GameObject::GetComponent(ComponentType component)
{
	for (size_t i = 0; i < components.size(); i++)
	{
		if (components[i]->GetType() == component)
		{
			return components[i];
		}
	}

	return nullptr;
}

std::vector<ModuleComponent*> GameObject::GetComponents()
{
	return components;
}

ModuleComponent* GameObject::AddComponent(ComponentType type)
{
	ModuleComponent* ModuleComponent = nullptr;

	switch (type)
	{
	case TRANSFORM:
		if (transform != nullptr)
		{
			RemoveComponent(transform);
		}

		transform = new ModuleTransform();
		ModuleComponent = transform;
		break;
	case MESH:
		ModuleComponent = new GnMesh();
		break;
	case MATERIAL:
		ModuleComponent = new ModuleMaterial(this);
		break;
	case CAMERA:
		ModuleComponent = new Camera(this);
		break;
	case LIGHT:
		ModuleComponent = new Light(this);
		break;
	default:
		break;
	}

	ModuleComponent->SetGameObject(this);
	components.push_back(ModuleComponent);

	return ModuleComponent;
}

void GameObject::AddComponent(ModuleComponent* component)
{
	components.push_back(component);
	component->SetGameObject(this);
}

bool GameObject::RemoveComponent(ModuleComponent* component)
{
	bool ret = false;

	for (size_t i = 0; i < components.size(); i++)
	{
		if (components[i] == component) {
			delete components[i];
			components.erase(components.begin() + i);
			component = nullptr;
			ret = true;
		}
	}

	return ret;
}

const char* GameObject::GetName() { return name.c_str(); }

void GameObject::SetName(const char* g_name)
{
	name = g_name;
}

void GameObject::SetTransform(ModuleTransform g_transform)
{
	//localTransform->Set(g_transform.GetLocalTransform());
	//localTransform->UpdateLocalTransform();
	memcpy(transform, &g_transform, sizeof(g_transform));
}

ModuleTransform* GameObject::GetTransform()
{
	return transform;
}

AABB GameObject::GetAABB()
{
	return _AABB;
}

bool GameObject::IsVisible()
{
	return _visible;
}

void GameObject::AddChild(GameObject* child)
{
	if (child != nullptr)
		children.push_back(child);

	child->SetParent(this);
}

int GameObject::GetChildrenAmount()
{
	return children.size();
}

GameObject* GameObject::GetChildAt(int index)
{
	return children[index];
}

GameObject* GameObject::GetParent()
{
	return _parent;
}

void GameObject::SetParent(GameObject* g_parent)
{
	_parent = g_parent;
}

void GameObject::Reparent(GameObject* newParent)
{
	if (newParent != nullptr)
	{
		_parent->RemoveChild(this);
		_parent = newParent;
		newParent->AddChild(this);
		transform->ChangeParentTransform(newParent->GetTransform()->GetGlobalTransform());
	}
}

bool GameObject::RemoveChild(GameObject* gameObject)
{
	bool ret = false;
	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i] == gameObject)
		{
			children.erase(children.begin() + i);
			ret = true;
		}
	}
	return ret;
}

void GameObject::DeleteChildren()
{
	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->DeleteChildren();
		children[i] = nullptr;
	}

	this->~GameObject();
}

void GameObject::UpdateChildrenTransforms()
{
	transform->UpdateGlobalTransform();

	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->GetTransform()->UpdateGlobalTransform(transform->GetGlobalTransform());
		children[i]->UpdateChildrenTransforms();
	}
}

void GameObject::GenerateAABB(GnMesh* mesh)
{
	_OBB = mesh->GetAABB();
	_OBB.Transform(transform->GetGlobalTransform());

	_AABB.SetNegativeInfinity();
	_AABB.Enclose(_OBB);

	float3 cornerPoints[8];
	_AABB.GetCornerPoints(cornerPoints);
	App->renderer3D->DrawAABB(cornerPoints);
}
