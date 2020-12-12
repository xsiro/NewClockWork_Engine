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
#include "ModuleResources.h"
#include "FileSys.h"

struct Hardware
{
	int cpu_count;
	int cache;
	float ram;
	const char* gpu;
	const char* gpu_brand;
	float vram_budget;
	float vram_usage;
	float vram_available;
	float vram_reserved;

	bool RDTSC = false;
	bool MMX = false;
	bool SSE = false;
	bool SSE2 = false;
	bool SSE3 = false;
	bool SSE41 = false;
	bool SSE42 = false;
	bool AVX = false;
	bool AVX2 = false;
	bool AltiVec = false;

	std::string caps;
};

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
	ModuleResources* res;
	bool in_game;
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
	float GetFPS();
	float GetLastDt();
	int GetFPSMax();
	void SetFPSMax(int max_fps);
	Hardware GetHardware();

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