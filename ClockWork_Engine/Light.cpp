#include "Globals.h"
#include "Light.h"
#include <gl/GL.h>
#include "imgui.h"


Light::Light() : ModuleComponent(), ref(-1), position(0.0f, 0.0f, 0.0f)
{
	type = ComponentType::LIGHT;
}

Light::Light(GameObject* gameObject) : ModuleComponent(gameObject), ref(-1), position(0.0f, 0.0f, 0.0f)
{
	type = ComponentType::LIGHT;
}

Light::~Light()
{
}

void Light::Init()
{
	glLightfv(ref, GL_AMBIENT, &ambient);
	glLightfv(ref, GL_DIFFUSE, &diffuse);
}

void Light::Update()
{
	Render();
}

void Light::SetPos(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Light::Render()
{
	if (enabled)
	{
		float pos[] = { position.x, position.y, position.z, 1.0f };
		glLightfv(ref, GL_POSITION, pos);
	}
}

void Light::OnEditor()
{
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float diffuse4f[4] = { diffuse.r, diffuse.g, diffuse.b, diffuse.a };
		if (ImGui::DragFloat3("Diffuse", diffuse4f, 0.1f, 0.0f, 1.0f))
		{
			diffuse.r = diffuse4f[0];
			diffuse.g = diffuse4f[1];
			diffuse.b = diffuse4f[2];
			diffuse.a = diffuse4f[3];
		}

		float ambient4f[4] = { ambient.r, ambient.g, ambient.b, ambient.a };
		if (ImGui::DragFloat3("Ambient", ambient4f, 0.1f, 0.0f, 1.0f))
		{
			ambient.r = ambient4f[0];
			ambient.g = ambient4f[1];
			ambient.b = ambient4f[2];
			ambient.a = ambient4f[3];
		}
	}
}


void Light::Active(bool active)
{
	enabled = active;

	if (enabled)
		glEnable(ref);
	else
		glDisable(ref);
}