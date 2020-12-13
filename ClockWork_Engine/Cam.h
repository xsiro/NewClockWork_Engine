#pragma once
#include "Globals.h"
#include "ModuleComponent.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;

enum FixedFOV {
	FIXED_VERTICAL_FOV,
	FIXED_HORIZONTAL_FOV
};

class Camera : public ModuleComponent {
public:
	Camera();
	Camera(GameObject* gameObject);
	~Camera();

	void Update() override;
	void OnEditor() override;

	

	void SetFixedFOV(FixedFOV fixedFOV);
	void AdjustFieldOfView();
	void AdjustFieldOfView(float width, float height);
	void SetVerticalFieldOfView(float verticalFOV, float screen_width, float screen_height);
	void SetHorizontalFieldOfView(float horizontalFOV, float screen_width = 16.0f, float screen_height = 9.0f);
	void SetPosition(float3 position);
	void SetReference(float3 reference);
	void SetNearPlaneDistance(float distance);
	void SetFarPlaneDistance(float distance);
	void Save(GnJSONArray& save_array) override;
	void Load(JSON& load_object) override;
	void Look(float3 spot);
	float3 GetPosition();
	float3 GetReference();
	Frustum GetFrustum();

	float* GetViewMatrix();
	float* GetProjectionMatrix();
	bool ContainsAABB(AABB& aabb);
	//virtual void Enable() override;
	//virtual void Disable() override;

public:
	FixedFOV fixedFOV;

private:
	Frustum _frustum;
	float _aspectRatio;
	float3 _reference;
};

