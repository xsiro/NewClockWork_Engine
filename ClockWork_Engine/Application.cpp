#include "Application.h"
#include "glew/include/glew.h"
#include "Globals.h"
#include "FileSys.h"
#include "Time.h"
#include "JSON.h"
#include <string>
#include "parson/parson.h"

Application::Application(int argc, char* args[]) : argc(argc), args(args), want_to_save(false), want_to_load(false), in_game(false)
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	scene = new ModuleSceneIntro(this);
	gui = new ModuleGui(this);
	res = new ModuleResources(this);

	// Main Modules
	AddModule(window);
	AddModule(res);
	AddModule(camera);
	AddModule(input);
	AddModule(scene);
	AddModule(gui);

	// Renderer last!
	AddModule(renderer3D);

	int cap = 60;
	max_ms = 1000 / cap;
}

Application::~Application()
{
	std::vector<Module*>::reverse_iterator item = modules_vector.rbegin();

	while (item != modules_vector.rend())
	{
		delete* item;
		++item;
	}
}

bool Application::Init()
{
	bool ret = true;

	FileSys::Init();
	

	char* buffer = nullptr;

	uint size = FileSys::Load("Library/Config/config.json", &buffer);
	JSON config(buffer);

	engine_name = config.GetString("engineName", "ClockWork_Engine");
	engine_version = config.GetString("version", "0.0");

	GnJSONArray modules_array(config.GetArray("modules_config"));

	// Call Init() in all modules
	for (size_t i = 0; i < modules_vector.size() && ret == true; i++)
	{
		JSON module_config(modules_array.GetObjectInArray(modules_vector[i]->name));

		ret = modules_vector[i]->LoadConfig(module_config);
		ret = modules_vector[i]->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (size_t i = 0; i < modules_vector.size() && ret == true; i++)
	{
		ret = modules_vector[i]->Start();
	}


	config.Release();
	RELEASE_ARRAY(buffer);

	
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	
	fps = 1.0f / dt;
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	Uint32 last_frame_ms;
	if (last_frame_ms < max_ms)
	{
		SDL_Delay(max_ms - last_frame_ms);
	}

	
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();

	for (size_t i = 0; i < modules_vector.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = modules_vector[i]->PreUpdate(dt);
	}

	for (size_t i = 0; i < modules_vector.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = modules_vector[i]->Update(dt);
	}

	for (size_t i = 0; i < modules_vector.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = modules_vector[i]->PostUpdate(dt);
	}

	if (!endFrameTasks.empty()) {
		for (size_t i = 0; i < endFrameTasks.size(); i++)
		{
			endFrameTasks.top()->OnFrameEnd();
			endFrameTasks.pop();
		}
	}

	if (want_to_save)
	{
		scene->Save(_file_to_save);
		want_to_save = false;
	}

	if (want_to_load)
	{
		scene->Load(_file_to_load);
		want_to_load = false;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	for (int i = modules_vector.size() - 1; i > 0; i--)
	{
		modules_vector[i]->CleanUp();
	}

	FileSys::CleanUp();

	return ret;
}


void Application::AddModule(Module* mod)
{
	modules_vector.push_back(mod);
}

float Application::GetFPS() { return fps; }

float Application::GetLastDt() { return dt; }

int Application::GetFPSMax()
{
	return 1000 / max_ms;
}

void Application::SetFPSMax(int fps_cap)
{
	max_ms = 1000.0f / (float)fps_cap;
}

void Application::Save(const char* filePath)
{
	want_to_save = true;
	//strcpy_s(_file_to_save, filePath);
}

void Application::Load(const char* filePath)
{
	want_to_load = true;
	//strcpy_s(_file_to_load, filePath);
}

void Application::AddModuleToTaskStack(Module* callback)
{
	endFrameTasks.push(callback);
}

Hardware Application::GetHardware()
{
	Hardware specs;

	//CPU
	specs.cpu_count = SDL_GetCPUCount();
	specs.cache = SDL_GetCPUCacheLineSize();

	//RAM
	specs.ram = SDL_GetSystemRAM() / 1000.0f;

	//Caps
	specs.RDTSC = SDL_HasRDTSC();
	specs.MMX = SDL_HasMMX();
	specs.SSE = SDL_HasSSE();
	specs.SSE2 = SDL_HasSSE2();
	specs.SSE3 = SDL_HasSSE3();
	specs.SSE41 = SDL_HasSSE41();
	specs.SSE42 = SDL_HasSSE42();
	specs.AVX = SDL_HasAVX();
	specs.AVX2 = SDL_HasAVX2();
	specs.AltiVec = SDL_HasAltiVec();

	if (specs.RDTSC) { specs.caps += "SDTSC"; }
	if (specs.MMX) { specs.caps += ", MMX"; }
	if (specs.SSE) { specs.caps += ", SSE"; }
	if (specs.SSE2) { specs.caps += ", SSE2"; }
	if (specs.SSE3) { specs.caps += ", SSE3"; }
	if (specs.SSE41) { specs.caps += ", SSE41"; }
	if (specs.SSE42) { specs.caps += ", SSE42"; }
	if (specs.AVX) { specs.caps += ", AVX2"; }
	if (specs.AltiVec) { specs.caps += ", AltiVec"; }

	//GPU
	//GLubyte* vendor = 
	specs.gpu = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	specs.gpu_brand = reinterpret_cast<const char*>(glGetString(GL_VENDOR));

	return specs;
}



