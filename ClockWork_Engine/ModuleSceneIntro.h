#pragma once
#include "Module.h"
#include "Globals.h"


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

private:

	std::vector<GameObject*> primitives;
};
