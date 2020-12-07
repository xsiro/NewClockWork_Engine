#include "Globals.h"
#include "Application.h"
#include "Module.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "imgui.h"
#include "ModuleGui.h"
#include "ModuleMesh.h"
#include "FileSys.h"
#include "GameObject.h"

class ModuleMesh;

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));
	GameObject* house = App->importer->LoadFBX("Assets/BakerHouse.FBX");
	App->importer->LoadTexture("Assets/BakerHouse.png");
	CreateGameObject(house);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	for (size_t i = 0; i < game_objects.size(); i++)
	{
		delete game_objects[i];
		game_objects[i] = nullptr;
	}
	game_objects.clear();

	return true;
}



// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();



	Cube cube(1.0f, 1.0f, 1.0f);
	if (App->gui->wireframe == true) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (App->gui->wireframe == false) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (App->gui->cube) {
		ModuleMesh* Cube = new ModuleMesh();
		ModuleMesh CreateCubeDirect();
	}

	if (App->gui->pyramid)
	{
		ModuleMesh* Pyramid = new ModuleMesh();
		ModuleMesh CreatePyramid();
	}

	if (App->gui->cylinder)
	{
		ModuleMesh* Cylinder = new ModuleMesh();
		ModuleMesh CreateCylinder();
	}

	if (App->gui->sphere)
	{
		ModuleMesh* Sphere = new ModuleMesh();
		ModuleMesh CreateSphere();
	}
	for (size_t i = 0; i < game_objects.size(); i++)
	{
		game_objects[i]->Update();
	}

	return UPDATE_CONTINUE;
}

GameObject* ModuleSceneIntro::CreateGameObject(GameObject* father) 
{
	GameObject* newo = new GameObject();
	newo->parent = father;
	game_objects.push_back(newo);

	selected_object = father;

	return newo;
}
