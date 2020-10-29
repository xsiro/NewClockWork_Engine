#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void RenderFBX();
	void LoadFBXBuffer();
	void DrawVertexNormalLines();
	void DrawFaceNormalLines();
	void OnResize(int width, int height);
	void SetDepthtest(bool state);
	void SetCullface(bool state);
	void SetLighting(bool state);
	void SetColormaterial(bool state);
	void SetTexture2D(bool state);
	void CreateCube();
	void CreateVertexCube();
	void CreateIndexCube();

	int num_indices = 18;

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
	
};