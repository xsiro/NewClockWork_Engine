#pragma once
#include <vector>
#include <string>
#include "glew/include/glew.h"

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleSceneIntro.h"
#include "ModulePhysics3D.h"
#include "ModuleGui.h"
#include "ModuleImporter.h"
#include "FileSys.h"



class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleSceneIntro* scene;
	ModulePhysics3D* physics;
	ModuleGui* gui;
	ModuleImporter* importer;
	FileSystem* filesys;
	
private:

	Timer	ms_timer;
	
	std::vector<Module*> modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	int CPUCount();
	int CPUCache();
	int SystemRAM();
	const char* SystemCaps();
	const char* Brand();
	const char* Model();
	int Budget();
	int Usage();
	int Available();
	int Reserved();
	std::string Caps;
	

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

public: 
	float max_ms;
	float fps;
	float dt;
};

extern Application* App;
extern std::vector<std::string> log_record;