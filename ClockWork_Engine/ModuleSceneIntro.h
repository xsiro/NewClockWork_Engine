#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "Primitive.h"
#include "glew/include/glew.h"
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
	GameObject* GetGameObject(const char* name);
	GameObject* GetGameObject(int id);

	void SelectGameObject(GameObject* selected);
	GameObject* GetSelectedGameObject();

	bool DeleteGameObject(GameObject* todelete);



	GameObject* root; 
	std::vector<GameObject*> game_objects;
	GameObject* selected_object;
	std::vector<int*> goID;
	int currentID;

	std::string name;
};
