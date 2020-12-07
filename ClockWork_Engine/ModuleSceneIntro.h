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
	update_status Update(float dt) ;
	bool CleanUp();
	GameObject* CreateGameObject(GameObject* GameObject);

	std::vector<GameObject*> game_objects;
	GameObject* selected_object;
	
	std::string name;
};
