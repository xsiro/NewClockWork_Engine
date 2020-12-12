#include "Inspector.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"

Inspector::Inspector() : Window()
{
	type = WindowType::INSPECTOR_WINDOW;
}

Inspector::~Inspector()
{
}

void Inspector::Draw()
{
	if (ImGui::Begin("Inspector", &visible))
	{
		if (App->scene->selected_object != nullptr)
		{
			App->scene->selected_object->OnEditor();
		}

		if (ImGui::CollapsingHeader("Resources")) {
			App->res->OnEditor();
		}
	}
	ImGui::End();
}
