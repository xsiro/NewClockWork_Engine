#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleMesh.h"
#include "FileSys.h"
#include "GameObject.h"
#include "ModuleTransform.h"

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled), show_grid(true), selected_object(nullptr), root(nullptr)
{
	name = "scene";

	mCurrentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
	mCurrentGizmoMode = ImGuizmo::MODE::WORLD;
}

ModuleSceneIntro::~ModuleSceneIntro() {}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	root = new GameObject();
	selected_object = root;
	root->SetName("Root");

	//GameObject* baker_house = App->resources->RequestGameObject("Assets/Models/baker_house/BakerHouse.fbx");
	//AddGameObject(baker_house);

	GameObject* Baker = App->res->RequestGameObject("Assets/BakerHouse.fbx");
	AddGameObject(Baker);

	GameObject* street_environment = App->res->RequestGameObject("Assets/street/Street environment_V01.fbx");
	//GameObject* street_environment = App->resources->RequestGameObject("Assets/Models/street2/street2.fbx");
	AddGameObject(street_environment);

	GameObject* camera = new GameObject();
	camera->AddComponent(ComponentType::CAMERA);
	camera->SetName("Camera");
	camera->GetTransform()->SetPosition(float3(0.0f, 0.0f, -5.0f));
	AddGameObject(camera);
	App->renderer3D->SetMainCamera((Camera*)camera->GetComponent(ComponentType::CAMERA));

	//uint baker_house_texture = App->resources->ImportFile("Assets/Textures/Baker_house.png");

	return ret;
}

bool ModuleSceneIntro::Init()
{
	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	if (show_grid)
	{
		GnGrid grid(24);
		grid.Render();
	}

	HandleInput();

	root->Update();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::HandleInput()
{
	if ((App->input->GetKey(SDL_SCANCODE_DELETE) == KEY_DOWN) && (selected_object != nullptr) && (selected_object != root))
		selected_object->to_delete = true;

	if ((App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN))
		mCurrentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;

	else if ((App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN))
		mCurrentGizmoOperation = ImGuizmo::OPERATION::ROTATE;

	else if ((App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN))
		mCurrentGizmoOperation = ImGuizmo::OPERATION::SCALE;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	root->DeleteChildren();
	delete root;
	root = nullptr;

	selected_object = nullptr;

	return true;
}

void ModuleSceneIntro::AddGameObject(GameObject* gameObject)
{
	if (gameObject != nullptr)
	{
		gameObject->SetParent(root);
		root->AddChild(gameObject);

		selected_object = gameObject;
	}
}

void ModuleSceneIntro::DeleteGameObject(GameObject* gameObject)
{
	if (root->RemoveChild(gameObject))
	{
		gameObject->DeleteChildren();
	}
	else if (gameObject->GetParent()->RemoveChild(gameObject))
	{
		gameObject->DeleteChildren();
	}

	delete gameObject;
	gameObject = nullptr;
}

std::vector<GameObject*> ModuleSceneIntro::GetAllGameObjects()
{
	std::vector<GameObject*> gameObjects;

	PreorderGameObjects(root, gameObjects);

	return gameObjects;
}

void ModuleSceneIntro::PreorderGameObjects(GameObject* gameObject, std::vector<GameObject*>& gameObjects)
{
	gameObjects.push_back(gameObject);

	for (size_t i = 0; i < gameObject->GetChildrenAmount(); i++)
	{
		PreorderGameObjects(gameObject->GetChildAt(i), gameObjects);
	}
}

void ModuleSceneIntro::EditTransform()
{
	if (selected_object == nullptr)
		return;

	float4x4 viewMatrix = App->camera->GetViewMatrixM().Transposed();
	float4x4 projectionMatrix = App->camera->GetProjectionMatrixM().Transposed();
	float4x4 objectTransform = selected_object->GetTransform()->GetGlobalTransform().Transposed();

	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(App->gui->sceneWindowOrigin.x, App->gui->sceneWindowOrigin.y, App->gui->image_size.x, App->gui->image_size.y);

	float tempTransform[16];
	memcpy(tempTransform, objectTransform.ptr(), 16 * sizeof(float));

	ImGuizmo::Manipulate(viewMatrix.ptr(), projectionMatrix.ptr(), mCurrentGizmoOperation, mCurrentGizmoMode, tempTransform);

	if (ImGuizmo::IsUsing())
	{
		float4x4 newTransform;
		newTransform.Set(tempTransform);
		objectTransform = newTransform.Transposed();
		selected_object->GetTransform()->SetGlobalTransform(objectTransform);
	}
}

bool ModuleSceneIntro::ClearScene()
{
	bool ret = true;

	root->DeleteChildren();
	root = nullptr;

	return ret;
}






