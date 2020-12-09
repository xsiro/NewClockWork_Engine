#include "ModuleTransform.h"
#include "imgui.h"
#include "Globals.h"

ModuleTransform::ModuleTransform() : ModuleComponent()
{
	position = { 0,0,0 };
	rotation = Quat::identity;
	scale = { 1,1,1 };
	UpdateLocalTransform();
	globalTransform = localTransform;
}

ModuleTransform::~ModuleTransform()
{
	CleanUp();
}

void ModuleTransform::SetTransform(float4x4 transform)
{
	transform.Decompose(position, rotation, scale);
	GenerateEulerFromRot();
}

void ModuleTransform::SetTransform(float3 pos, Quat rot, float3 scl)
{
	this->position = pos;
	this->rotation = rot;
	this->scale = scl;
	UpdateLocalTransform();
	GenerateEulerFromRot();
}

void ModuleTransform::SetTransform(float3 pos, float3 rot, float3 scl)
{
	this->position = pos;
	this->rotation = Quat::FromEulerXYZ(DegToRad(rot.x), DegToRad(rot.y), DegToRad(rot.z));
	this->scale = scl;
	UpdateLocalTransform();
	GenerateEulerFromRot();
}

void ModuleTransform::Update()
{

}

void ModuleTransform::CleanUp()
{

}

float4x4 ModuleTransform::GetLocalTransform()
{
	return localTransform;
}

float4x4 ModuleTransform::GetGlobalTransform()
{
	return globalTransform;
}

void ModuleTransform::UpdateLocalTransform()
{
	localTransform = float4x4::FromTRS(position, rotation, scale);
	//globalTransform = localTransform;
}

void ModuleTransform::UpdateGlobalTransform(float4x4 parentGlobalTransform)
{
	localTransform = float4x4::FromTRS(position, rotation, scale);
	globalTransform = parentGlobalTransform * localTransform;
}

float3 ModuleTransform::GetPosition() const
{
	return position;
}

Quat ModuleTransform::GetRotation() const
{
	return rotation;
}

float3 ModuleTransform::GetRotationEuler() const
{
	return rotationEuler;
}

float3 ModuleTransform::GetScale() const
{
	return scale;
}

void ModuleTransform::GenerateEulerFromRot()
{
	rotationEuler = rotation.ToEulerXYZ();
	rotationEuler *= RADTODEG;
}