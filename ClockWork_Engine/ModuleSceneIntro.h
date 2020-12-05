#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui.h"


class GameObject;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();
	
	bool Start();
	update_status Update(float dt) override;


	bool CleanUp();
	GameObject* CreateGameObject(GameObject* GameObject);


	GameObject* selected_object = nullptr;
	std::vector<GameObject*> game_objects;
	
};
