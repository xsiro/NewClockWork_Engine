#ifndef __ModuleGui_H__
#define __ModuleGui_H__

#include "Module.h"
#include <list>
#include <string>
#include <vector>
#include "imgui.h"
#include "glmath.h"

#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL_video.h"

#define CURSOR_WIDTH 2
#define MAX_LOGS 30

struct SDL_Texture;
struct _TTF_Font;
struct SDL_Rect;
struct SDL_Color; 

enum UIELEMENT_TYPES
{
	BUTTON,
	CHECKBOX,
	INPUTTEXT,
	LABEL,
	IMAGE,
	BOX
};

// ---------------------------------------------------
class ModuleGui : public Module
{
public:

	ModuleGui(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleGui();

	// Called when before render is available
	bool Awake();

	// Call before first frame
	bool Init();

	// Called before all Updates
	update_status PreUpdate(float dt);

	// Called every frame
	update_status Update(float dt);

	// Called after all Updates
	update_status PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();
	
	update_status Dock(bool* p_open);
	
	

public:

	//our state
	bool show_demo_window = false;
	bool mainwindow;
	bool viewconfig;
	bool viewconsole;
	bool inspector;
	bool hierarchy;
	bool depthtest;
	bool cullface;
	bool lighting;
	bool polygonssmooth;
	bool material;
	bool texture2D;
	bool cubemap;
	bool checker;
	bool console = false;
	bool about;
	bool wireframe;
	bool cube;
	bool pyramid;
	bool cylinder;
	bool sphere;
	bool showmaterial;
	bool show_another_window = false;
	void RequestBrowser(const char*);
	void ClearLog();
	void AddLogText(std::string incoming_text);
	void ConsoleLog(char* log);
	void Hierarchy();
	bool show_configuration_window;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	int fps;
	int height;
	int width;
	bool fullscreen;
	bool resizable;
	bool borderless;
	bool fulldesktop;
	bool vertexlines;
	bool facelines;
	bool check;
	float brightness;
	const char* GetName() const;
	const char* name;
	bool scroll;


	SDL_GLContext gl_context; 
	//ImGuiIO* io = nullptr;
	std::vector<float> fps_log;
	std::vector<float> ms_log;
	bool* dockingwindow;
	std::vector<std::string> log_record;
	std::vector<char*> logs;

};

#endif // __ModuleGui_H__