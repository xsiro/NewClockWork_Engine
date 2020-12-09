#pragma once
#pragma once
#include "Globals.h"
#include <vector>
#include "ModuleComponent.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;

class ModuleTransform : public ModuleComponent
{
public:

	ModuleTransform();
	~ModuleTransform();

	void Update() override;

	void SetTransform(float4x4 transform);
	void SetTransform(float3 pos, Quat rotation, float3 scale);
	void SetTransform(float3 pos, float3 rotation, float3 scale);

	void CleanUp();

	float4x4	GetLocalTransform();
	float4x4	GetGlobalTransform();
	void		UpdateLocalTransform();
	void		UpdateGlobalTransform(float4x4);

	float3		GetPosition() const;
	Quat		GetRotation() const;
	float3		GetRotationEuler() const;
	float3		GetScale() const;

private:

	void GenerateEulerFromRot();

	//Transformation values
	float4x4 localTransform;
	float4x4 globalTransform;
	float3 position;
	Quat rotation;
	float3 scale;
	float3 rotationEuler;

};