#ifndef _MODULE_GUI_H_
#define _MODULE_GUI_H_

#include "Module.h"
#include "Globals.h"

#include "imgui.h"
#include "Window.h"

#include <vector>
#include <string>

typedef int GLint;

struct log_message {
	std::string log_text;
	int warning_level;
};

class ModuleGui : public Module
{
public:
	ModuleGui(bool start_enabled = true);
	~ModuleGui();

	bool Init();
	
	update_status Update(float dt);
	update_status Draw();
	bool CleanUp();

	bool IsSceneFocused();
	bool MouseOnScene();
	void AddConsoleLog(const char* log, int warning_level);

	void OnResize(ImVec2 window_size);
	void LoadFile(const char* filter_extension, const char* from_dir);
	void SaveFile(const char* filter_extension, const char* from_dir);
	void DrawDirectoryRecursive(const char* directory, const char* filter_extension);

private:
	//Dock Space
	update_status ShowDockSpace(bool* p_open);
	bool CreateMainMenuBar();

	void ShowGameButtons();

	//Windows
	void ShowPreferencesWindow();

	void ChangeTheme(std::string theme);

public:
	ImVec2 image_size;
	ImVec2 sceneWindowOrigin;
	ImVec2 mouseScenePosition;
	bool scene_window_focused;
	Window* windows[MAX_WINDOWS];

private:

	bool show_project_window;
	bool show_console_window;

	//edit subwindows
	bool show_preferences_window;
	bool show_game_buttons;

	//menus
	bool* open_dockspace;

	int current_theme;

	std::vector<log_message> console_log;

	enum
	{
		closed,
		opened,
		ready_to_close
	} file_dialog = closed;

	enum class SceneOperation
	{
		SAVE,
		LOAD,
		NONE
	}scene_operation = SceneOperation::NONE;

	bool in_modal = false;
	char selected_file[256];
};

#endif