#include "ModuleTransform.h"
#include "imgui.h"
#include "Globals.h"
#include "GameObject.h"
#include "JSON.h"


ModuleTransform::ModuleTransform() : ModuleComponent()
{
	type = ComponentType::TRANSFORM;

	_position = float3::zero;
	_rotation = Quat::identity;
	_eulerRotation = float3::zero;
	_scale = float3::one;

	_localTransform = float4x4::FromTRS(_position, _rotation, _scale);
	_globalTransform = _localTransform;
	_parentGlobalTransform = float4x4::identity;
}

ModuleTransform::ModuleTransform(float3 position, Quat rotation, float3 scale) : ModuleComponent()
{
	type = ComponentType::TRANSFORM;

	_position = position;
	_rotation = rotation;
	_eulerRotation = rotation.ToEulerXYZ();
	_eulerRotation *= RADTODEG;
	_scale = scale;

	_localTransform = float4x4::FromTRS(_position, _rotation, _scale);
	_globalTransform = _localTransform;
	_parentGlobalTransform = float4x4::identity;
}

ModuleTransform::~ModuleTransform() {}

void ModuleTransform::Update() {}

void ModuleTransform::Save(GnJSONArray& save_array)
{
	JSON save_object;
	save_object.AddInt("Type", type);

	save_object.AddFloat3("Position", _position);

	save_object.AddQuaternion("Rotation", _rotation);

	save_object.AddFloat3("Scale", _scale);

	save_array.AddObject(save_object);
}

void ModuleTransform::Load(JSON& load_object)
{
	_position = load_object.GetFloat3("Position");
	_rotation = load_object.GetQuaternion("Rotation");
	_scale = load_object.GetFloat3("Scale");

	UpdateEulerRotation();
	UpdateGlobalTransform();
}

void ModuleTransform::OnEditor()
{
	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float position4f[4] = { _position.x, _position.y, _position.z, 1.0f };
		if (ImGui::DragFloat3("Position", position4f, 0.1f, -10000.0f, 10000.0f))
		{
			SetPosition(position4f[0], position4f[1], position4f[2]);
			UpdateGlobalTransform();
			_gameObject->UpdateChildrenTransforms();
		}

		float rotation4f[4] = { _eulerRotation.x, _eulerRotation.y, _eulerRotation.z, 1.0f };
		if (ImGui::DragFloat3("Rotation", rotation4f, 0.1f, -360.0f, 360.0f))
		{
			SetRotation(rotation4f[0], rotation4f[1], rotation4f[2]);
			UpdateGlobalTransform();
			_gameObject->UpdateChildrenTransforms();
		}

		float scale4f[4] = { _scale.x, _scale.y, _scale.z, 1.0f };
		if (ImGui::DragFloat3("Scale", scale4f, 0.01f, -10000.0f, 10000.0f))
		{
			SetScale(scale4f[0], scale4f[1], scale4f[2]);
			UpdateGlobalTransform();
			_gameObject->UpdateChildrenTransforms();
		};

		ImGui::Spacing();

		if (ImGui::Button("Reset"))
			Reset();

		ImGui::Spacing();
	}
}



void ModuleTransform::Set(float4x4 g_transform)
{
	_localTransform = g_transform;
}

float4x4 ModuleTransform::GetLocalTransform()
{
	return _localTransform;
}

float4x4 ModuleTransform::GetGlobalTransform()
{
	return _globalTransform;
}

void ModuleTransform::SetGlobalTransform(float4x4 newTransform)
{
	_globalTransform = newTransform;
	float4x4 inverseParentGlobal = _parentGlobalTransform;
	inverseParentGlobal.Inverse();
	_localTransform = inverseParentGlobal * _globalTransform;
	UpdateTRS();
	_gameObject->UpdateChildrenTransforms();
}

void ModuleTransform::UpdateLocalTransform()
{
	_localTransform = float4x4::FromTRS(_position, _rotation, _scale);
	UpdateEulerRotation();
}

void ModuleTransform::UpdateTRS()
{
	_localTransform.Decompose(_position, _rotation, _scale);
	UpdateEulerRotation();
}

void ModuleTransform::UpdateGlobalTransform()
{
	UpdateLocalTransform();
	_globalTransform = _parentGlobalTransform * _localTransform;
}

void ModuleTransform::UpdateGlobalTransform(float4x4 parentGlobalTransform)
{
	UpdateLocalTransform();
	_parentGlobalTransform = parentGlobalTransform;
	_globalTransform = _parentGlobalTransform * _localTransform;
}

void ModuleTransform::ChangeParentTransform(float4x4 newParentGlobalTransform)
{
	_parentGlobalTransform = newParentGlobalTransform;
	newParentGlobalTransform.Inverse();
	_localTransform = newParentGlobalTransform * _globalTransform;
	UpdateTRS();
	_globalTransform = _parentGlobalTransform * _localTransform;
}

void ModuleTransform::Reset()
{
	_position = float3::zero;
	_rotation = Quat::identity;
	_scale = float3::one;
	_eulerRotation = float3::zero;

	UpdateGlobalTransform();
	_gameObject->UpdateChildrenTransforms();

}

void ModuleTransform::SetPosition(float x, float y, float z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;

	UpdateLocalTransform();
}

void ModuleTransform::SetPosition(float3 new_position)
{
	_position = new_position;
	UpdateGlobalTransform();
}

float3 ModuleTransform::GetPosition()
{
	return _position;
}


//Set rotation from Euler angles
void ModuleTransform::SetRotation(float x, float y, float z)
{
	_eulerRotation = float3(x, y, z);
	_rotation = Quat::FromEulerXYZ(x * DEGTORAD, y * DEGTORAD, z * DEGTORAD);
	UpdateGlobalTransform();
}

void ModuleTransform::SetRotation(Quat new_rotation)
{
	_rotation = new_rotation;
	_eulerRotation = _rotation.ToEulerXYZ() * RADTODEG;

	UpdateGlobalTransform();
}

void ModuleTransform::SetRotation(float x, float y, float z, float w)
{
	_rotation.x = x;
	_rotation.y = y;
	_rotation.z = z;
	_rotation.w = w;

	UpdateGlobalTransform();
}
//
//Quat ModuleTransform::GetRotation()
//{
//	return _rotation;
//}

void ModuleTransform::UpdateEulerRotation()
{
	_eulerRotation = _rotation.ToEulerXYZ();
	_eulerRotation *= RADTODEG;
}

void ModuleTransform::SetScale(float x, float y, float z)
{
	_scale.x = x;
	_scale.y = y;
	_scale.z = z;

	UpdateGlobalTransform();
}

void ModuleTransform::SetScale(float3 new_scale)
{
	_scale = new_scale;

	UpdateGlobalTransform();
}

void ModuleTransform::SetProportionalScale(float multiplier)
{
	_scale.x = _scale.y = _scale.z = multiplier;

	UpdateGlobalTransform();
}

float3 ModuleTransform::GetScale()
{
	return _scale;
}

