#include "Wscene.h"
#include "imgui.h"
#include "Application.h"
#include "glew/include/glew.h"
#include "Assets.h"
#include "Time.h"


Scene::Scene() : Window()
{
	type = WindowType::SCENE_WINDOW;
}

Scene::~Scene()
{
}

void Scene::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	if (ImGui::Begin("Scene", &visible, ImGuiWindowFlags_MenuBar))
	{
		App->gui->scene_window_focused = ImGui::IsWindowFocused();

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Display"))
			{
				if (ImGui::BeginMenu("Shading Mode"))
				{
					if (ImGui::MenuItem("Solid", NULL, App->renderer3D->display_mode == DisplayMode::SOLID))
						App->renderer3D->SetDisplayMode(DisplayMode::SOLID);
					if (ImGui::MenuItem("Wireframe", NULL, App->renderer3D->display_mode == DisplayMode::WIREFRAME))
						App->renderer3D->SetDisplayMode(DisplayMode::WIREFRAME);
					ImGui::EndMenu();
				}

				static bool vertex_normals = App->renderer3D->draw_vertex_normals;
				if (ImGui::Checkbox("Vertex Normals", &vertex_normals))
					App->renderer3D->draw_vertex_normals = vertex_normals;

				static bool face_normals = App->renderer3D->draw_face_normals;
				if (ImGui::Checkbox("Face Normals", &face_normals))
					App->renderer3D->draw_face_normals = face_normals;

				ImGui::EndMenu();
			}

			static bool lighting = glIsEnabled(GL_LIGHTING);
			if (ImGui::Checkbox("Lighting", &lighting))
				App->renderer3D->SetCapActive(GL_LIGHTING, lighting);

			static bool show_grid = App->scene->show_grid;
			if (ImGui::Checkbox("Show Grid", &show_grid))
				App->scene->show_grid = show_grid;

			ImGui::Checkbox("Cull editor camera", &App->renderer3D->cullEditorCamera);

			ImGui::EndMenuBar();
		}

		ImVec2 window_size = ImGui::GetContentRegionAvail();
		App->gui->sceneWindowOrigin = ImGui::GetWindowPos();
		App->gui->sceneWindowOrigin.x += ImGui::GetWindowContentRegionMin().x;
		App->gui->sceneWindowOrigin.y += ImGui::GetWindowContentRegionMin().y;

		App->gui->mouseScenePosition.x = App->input->GetMouseX() - App->gui->sceneWindowOrigin.x;
		App->gui->mouseScenePosition.y = App->input->GetMouseY() - App->gui->sceneWindowOrigin.y;

		/*if (App->in_game)
			DrawGameTimeDataOverlay();*/

		if (App->gui->image_size.x != window_size.x || App->gui->image_size.y != window_size.y)
			App->gui->OnResize(window_size);

		ImGui::Image((ImTextureID)App->renderer3D->colorTexture, App->gui->image_size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::PushID(SCENE_WINDOW);
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS"))
			{
				IM_ASSERT(payload->DataSize == sizeof(int));
				int payload_n = *(const int*)payload->Data;
				Assets* assets_window = (Assets*)App->gui->windows[ASSETS_WINDOW];
				const char* file = assets_window->GetFileAt(payload_n);
				App->scene->AddGameObject(App->res->RequestGameObject(file));
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::PopID();

		App->scene->EditTransform();
		//ImGui::Image((ImTextureID)App->renderer3D->depthTexture, image_size, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
	}
	ImGui::End();
	ImGui::PopStyleVar();
}

