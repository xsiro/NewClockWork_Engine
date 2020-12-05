#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "imgui.h"
#include "ModuleGui.h"

class ComponentMesh;

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
	App->importer->LoadTexture("Assets/BakerHouseIMG.png");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}



// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 0, 1, 0);
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
		ComponentMesh CreateCubeDirect();
	}

	if (App->gui->pyramid)
	{
		ComponentMesh CreatePyramid();
	}

	if (App->gui->cylinder)
	{
		ComponentMesh CreateCylinder();
	}

	if (App->gui->sphere)
	{
		ComponentMesh CreateSphere();
	}

	return UPDATE_CONTINUE;
}

GameObject* ModuleSceneIntro::CreateGameObject(GameObject* GameObject) 
{
	return GameObject;
}
