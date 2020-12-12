#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();
	void SetBackgroundColor(float r, float g, float b, float w);
	FixedFOV GetFixedFOV();
	void SetFixedFOV(FixedFOV fixedFOV);
	float GetVerticalFieldOfView();
	float GetHorizontalFieldOfView();
	void SetVerticalFieldOfView(float verticalFOV, int screen_width, int screen_height);
	void SetHorizontalFieldOfView(float horizontalFOV, int screen_width, int screen_height);
	void OnResize(int width, int height);
private:

	void CalculateViewMatrix();

public:
	Camera* _camera;
	Color background;
	vec3 X, Y, Z, Position, Reference;
	float move_speed;
	float drag_speed;
	float orbit_speed;
	float zoom_speed;
	float sensitivity;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
};