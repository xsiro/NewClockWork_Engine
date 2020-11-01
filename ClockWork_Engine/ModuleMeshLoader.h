/*#pragma once
#include "Globals.h"
#include "glmath.h"
#include <vector>
#include "ModuleImporter.h"

class GameObject;

enum class ComponentType {
	Mesh,
	Transform,
	Material
};

class Component
{
public:

	Component();
	~Component();

	virtual void Enable();
	virtual void Update();
	virtual void Disable();

	ComponentType ReturnType();
	GameObject* ReturnGameObject();


public:

	bool active;
	GameObject* owner;
	ComponentType type;


};

class ModuleMesh : Component
{
public:

	ModuleMesh();
	~ModuleMesh();

	void CreateCubeDirect();
	void CreateCubeVertex();
	void CreateCubeIndex();
	void CreatePyramid();
	void CreateSphere(float radius, unsigned int rings, unsigned int sectors);
	void CreateCylinder(float radius, float height, int sides);

	void RenderFBX();
	void LoadFBXBuffer();
	void DrawVertexNormalLines();
	void DrawFaceNormalLines();
	void LoadingTextures();

public:

	/*GLuint Gl_Tex;
	GLuint lenna_texture;
	bool rendered;
	GLubyte checkerImage[64][64][4];*/

/*public:

	uint id_index = 0; 
	uint num_index = 0;
	uint* index = nullptr;

	uint	id_normals = 0;
	uint	num_normals = 0;
	float* normals = NULL;

	uint	id_colors = 0;
	uint	num_colors = 0;
	float* colors = NULL;

	uint	id_texcoords = 0;
	uint	num_texcoords = 0;
	float* texcoords = nullptr;
	uint image_id;

	uint id_vertex = 0;
	uint num_vertex = 0;
	float* vertex = nullptr;


};*/