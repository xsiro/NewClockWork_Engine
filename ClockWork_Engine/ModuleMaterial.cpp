#include "Application.h"
#include "Globals.h"
#include "ModuleMaterial.h"
#include "imgui.h"
#include "glew/include/glew.h"
#include "ModuleImporter.h"

#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib") 
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glew/libx86/glew32.lib")

ModuleMaterial::ModuleMaterial() : ModuleComponent()
{
	type = ComponentType::Material;
	Gl_Tex = 0;
}

ModuleMaterial::~ModuleMaterial()
{
}

void ModuleMaterial::Update()
{

}