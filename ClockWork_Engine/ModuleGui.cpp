#include "Application.h"
#include "ModuleInput.h"
#include "ModuleGui.h"
#include "ModuleAudio.h"
#include "ModuleWindow.h"
#include "Primitive.h"


#include <stdio.h>
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "gl3w.h"


ModuleGui::ModuleGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{

	brightness = 1.0f;
	width = 1280;
	height = 1024;
	fps = 0;
	fullscreen = false;
	resizable = false;
	borderless = true;
	fulldesktop = false;

	fps_log = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
	ms_log = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	viewconfig = true;
	console = true;
	about = false;
}

// Destructor
ModuleGui::~ModuleGui()
{}

// Called before render is available
bool ModuleGui::Awake()
{
	LOG("Loading GUI atlas");
	bool ret = true;	

	return ret;
}

// Called before the first frame
bool ModuleGui::Start()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	(void)io;

	// Setting context
	gl_context = SDL_GL_CreateContext(App->window->window);
	SDL_GL_MakeCurrent(App->window->window, gl_context);

	//TEST
	io = &ImGui::GetIO(); (void)io;
	//TEST

	//TEST
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit() != 0;
#endif
	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}

	//TEST

	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark(); 
	
	//const char* glsl_version = "#version 130";

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'misc/fonts/README.txt' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal 
	io->Fonts->AddFontDefault();
	io->Fonts->AddFontFromFileTTF("misc/fonts/Roboto-Medium.ttf", 14);

	return true;
}

// Update all guis
update_status ModuleGui::PreUpdate(float dt)
{
	
	return  UPDATE_CONTINUE;
}

// Called every frame
update_status ModuleGui::Update(float dt)
{
	


	return  UPDATE_CONTINUE;
}

// Called after all Updates
update_status ModuleGui::PostUpdate(float dt)
{
	bool ret = true;
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit")) { return UPDATE_STOP; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Configuration"))
			{
				viewconfig = !viewconfig;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("GitHub")) RequestBrowser("https://github.com/xsiro/ClockWork_Engine");
			{
				ImGui::EndMenu();
			}
		}
		if (ImGui::BeginMenu("About"))
		{

			ImGui::OpenPopup("About");
			if (ImGui::BeginPopupModal("About"))
			{
				ImGui::Text("ClockWorkEngine is developed by Daniel Ruiz & Pol Cort�s");
				ImGui::Text("This engine has been coded in C++");
				ImGui::Text("Libraries:");
				ImGui::Text("OpenGL v2.1");
				ImGui::Text("Glew v7.0");
				ImGui::Text("MathGeoLib v1.5");
				ImGui::Text("PhysFS v3.0.2");

				ImGui::Text("");

				ImGui::Text("LICENSE:");
				ImGui::Text("");
				ImGui::Text("MIT License");
				ImGui::Text("");
				ImGui::Text("Copyright (c) 2020 [Daniel Ruiz & Pol Cort�s]");
				ImGui::Text("");
				ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
				ImGui::Text("of this software and associated documentation files (the 'Software'), to deal");
				ImGui::Text("in the Software without restriction, including without limitation the rights");
				ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and/or sell");
				ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
				ImGui::Text("furnished to do so, subject to the following conditions:");
				ImGui::Text("");
				ImGui::Text("The above copyright notice and this permission notice shall be included in all");
				ImGui::Text("copies or substantial portions of the Software.");
				ImGui::Text("");
				ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
				ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
				ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE");
				ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
				ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
				ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN ");
				ImGui::Text("THE SOFTWARE.");

				if (ImGui::Button("Close"))
				{
					ImGui::CloseCurrentPopup();
					ret = false;
				}
				ImGui::EndPopup();
				ImGui::EndMenu();
			}

		}
		ImGui::EndMainMenuBar();

	}



	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	//if (mainwindow)
		//ImGui::ShowDemoWindow(&mainwindow);

	if (viewconfig)
	{
		ImGui::Begin("Configuration", &viewconfig);
		ImGui::Text("Options");
		if (ImGui::CollapsingHeader("Application"))
		{
			char Appname[32] = "ClockWork Engine";
			ImGui::InputText("App Name", Appname, IM_ARRAYSIZE(Appname));

			char Organization[32] = "CITM";
			ImGui::InputText("Organization", Organization, IM_ARRAYSIZE(Organization));

			ImVec4 color(1.0f, 0.0f, 1.0f, 1.0f);
			int mfps = 1000 / App->max_ms;

			if (ImGui::SliderInt("Max FPS", &mfps, 10, 120)) App->max_ms = 1000 / mfps;

			ImGui::Text("Limit Framerate: ");
			ImGui::SameLine();
			ImGui::TextColored(color, "%i", fps);
			fps_log.erase(fps_log.begin());
			fps_log.push_back(App->fps);
			ms_log.erase(ms_log.begin());
			ms_log.push_back(App->dt * 1000);

			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
			ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
			ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));

		}
		if (ImGui::CollapsingHeader("Window"))
		{

			ImGui::Text("Icon:  *default");

			ImGui::SliderFloat("Brightness", &brightness, 0.f, 1.0f);
			App->window->SetBright(brightness);

			ImGui::SliderInt("Width", &width, 640, 1920);
			App->window->SetWidth(width);

			ImGui::SliderInt("Height", &height, 480, 1080);
			App->window->SetHeight(height);

			if (ImGui::Checkbox("Fullscreen", &fullscreen));
			App->window->SetFullScreen(fullscreen);
			ImGui::SameLine();

			ImGui::Checkbox("Resizable", &resizable);
			App->window->SetResizable(resizable);

			ImGui::Checkbox("Borderless", &borderless);
			App->window->SetBorderless(borderless);
			ImGui::SameLine();

			ImGui::Checkbox("Fulldesktop", &fulldesktop);
			App->window->SetFullDesktop(fulldesktop);

			if (ImGui::IsAnyItemHovered())
				ImGui::SetTooltip("Restart to Apply");
		}
		if (ImGui::CollapsingHeader("Hardware"))
		{
			ImVec4 color(1.0f, 1.0f, 0.0f, 1.0f);
			ImGui::Text("SDL version: %d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
			ImGui::Separator();

			int cpu;
			int ram;
			int cache;
			ImGui::Text("CPUs:");
			ImGui::SameLine();
			ImGui::TextColored(color, "%i", cpu = App->CPUCount());
			ImGui::SameLine();
			ImGui::TextColored(color, "(Cache: %iKb)", cache = App->CPUCache());
			ImGui::Text("System RAM: ");
			ImGui::SameLine();
			ImGui::TextColored(color, "%i.0GB", ram = App->SystemRAM());
			ImGui::Text("Caps: ");
			ImGui::SameLine();
			ImGui::TextColored(color, "%s", App->SystemCaps());
			ImGui::Separator();

			ImGui::Text("Brand:");
			ImGui::SameLine();
			ImGui::TextColored(color, "%s", App->Brand());
			ImGui::Text("Model:");
			ImGui::SameLine();
			ImGui::TextColored(color, "%s", App->Model());
			ImGui::Text("VRAM Budget:");
			ImGui::SameLine();
			ImGui::TextColored(color, "%i Mb", App->Budget());
			ImGui::Text("VRAM Usage:");
			ImGui::SameLine();
			ImGui::TextColored(color, "%i Mb", App->Usage());
			ImGui::Text("VRAM Available:");
			ImGui::SameLine();
			ImGui::TextColored(color, "%i Mb", App->Available());
			ImGui::Text("VRAM Reserved:");
			ImGui::SameLine();
			ImGui::TextColored(color, "%i Mb", App->Reserved());
		}
		if (ImGui::CollapsingHeader("Renderer"))
		{

		}

		ImGui::End();

	}

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.

	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("Clockwork_engine");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text("YES");               // Display some text (you can use a format strings too)
	ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("Background color", (float*)&clear_color); // Edit 3 floats representing a color

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();


	// Rendering
	ImGui::Render();
	glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);
	//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	//SDL_GL_SwapWindow(App->window->window);	
	return  UPDATE_CONTINUE;
}


// Called before quitting
bool ModuleGui::CleanUp()
{
	LOG("Freeing Gui");
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_GL_DeleteContext(gl_context);

	return true;
}

void ModuleGui::RequestBrowser(const char* path)
{
	ShellExecuteA(0, "Open", path, 0, "", 5);
}

void ModuleGui::ClearLog()
{
	for (int i = 0; i < logs.size(); ++i)
	{
		free(logs[i]);
	}

	logs.clear();
}

bool ModuleGui::DrawConsole(ImGuiIO& io)
{
	bool ret = true;
	ImGuiWindowFlags win_flags = ImGuiWindowFlags_MenuBar;
	ImGui::Begin(GetName(), NULL, win_flags);
	ConsoleMenu();
	ConsoleOutput();
	ImGui::End();
	return ret;
}

void ModuleGui::ConsoleOutput()
{
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 6));

	for (int i = 0; i < logs.size(); ++i)
	{
		ImVec4 text_colour = { 1.0f, 1.0f, 1.0f, 1.0f };

		if (strstr(logs[i], "[ERROR]") != nullptr)
		{
			text_colour = { 1.0f, 0.0f, 0.0f, 1.0f };

			if (strstr(logs[i], "[WARNING]") != nullptr)
			{
				text_colour = { 1.0f, 1.0f, 0.0f, 1.0f };
			}

			ImGui::PushStyleColor(ImGuiCol_Text, text_colour);
			ImGui::TextUnformatted(logs[i]);
			ImGui::PopStyleColor();
		}
	}

	ImGui::PopStyleVar();
}

void ModuleGui::ConsoleMenu()
{
	ImGui::BeginMenuBar();														

	if (ImGui::BeginMenu("Options"))											
	{
		if (ImGui::MenuItem("Clear Console"))									
		{
			ClearLog();
		}

		if (ImGui::MenuItem("Close Console"))
		{
			
		}

		ImGui::EndMenu();														
	}

	ImGui::EndMenuBar();														
}

const char* ModuleGui::GetName() const
{
	return name;
}