#ifndef __ModuleGui_H__
#define __ModuleGui_H__

#include "Module.h"
#include <list>
#include <string>
#include "imgui.h"

#include "SDL/include/SDL_rect.h"
#include "SDL/include/SDL_video.h"

#define CURSOR_WIDTH 2

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
	bool Start();

	// Called before all Updates
	update_status PreUpdate(float dt);

	// Called every frame
	update_status Update(float dt);

	// Called after all Updates
	update_status PostUpdate(float dt);

	// Called before quitting
	bool CleanUp();

	void Draw();
	void ConsoleMenu();
	bool DrawConsole(ImGuiIO& io);
	

public:

	//our state
	bool show_demo_window = false;
	bool mainwindow;
	bool viewconfig;
	bool viewconsole;
	bool console;
	bool about;
	bool show_another_window = false;
	void RequestBrowser(const char*);
	void ClearLog();
	void ConsoleOutput();
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
	float brightness;
	void Log(const char* log);
	const char* GetName() const;
	const char* name;

private:

	SDL_GLContext gl_context; 
	ImGuiIO* io = nullptr;
	std::vector<float> fps_log;
	std::vector<float> ms_log;

	std::vector<std::string> log_record;
	std::vector<char*> logs;

};

#endif // __ModuleGui_H__