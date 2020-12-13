#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "ImGuizmo/ImGuizmo.h"

#include <vector>

class GameObject;
class GnTexture;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	bool Init();
	
	update_status Update(float dt);
	void HandleInput();
	bool CleanUp();
	bool LoadConfig(JSON& config) override;
	void AddGameObject(GameObject* gameObject);
	void DeleteGameObject(GameObject* gameObject);
	GameObject* GetRoot() { return root; }
	std::vector<GameObject*> GetAllGameObjects();
	void PreorderGameObjects(GameObject* gameObject, std::vector<GameObject*>& gameObjects);
	void EditTransform();

	bool ClearScene();

	bool Save(const char* file_path);
	bool Load(const char* scene_file);

public:
	bool show_grid;
	GameObject* selected_object;

private:
	GameObject* root;
	ImGuizmo::OPERATION mCurrentGizmoOperation;
	ImGuizmo::MODE mCurrentGizmoMode;
};
