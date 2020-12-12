#include "Hierarchy.h"
#include "imgui.h"
#include "GameObject.h"
#include "ModuleSceneintro.h"
#include "Application.h"

Hierarchy::Hierarchy() : Window()
{
	type = WindowType::HIERARCHY_WINDOW;
}

Hierarchy::~Hierarchy() {}

void Hierarchy::Draw()
{
	if (ImGui::Begin("Hierarchy", &visible))
	{
		GameObject* root = App->scene->GetRoot();
		int id = 0;
		PreorderHierarchy(root, id);
	}
	ImGui::End();
}

void Hierarchy::PreorderHierarchy(GameObject* gameObject, int& id)
{
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	if (gameObject == App->scene->GetRoot())
		flags |= ImGuiTreeNodeFlags_DefaultOpen;

	if (App->scene->selected_object == gameObject)
		flags |= ImGuiTreeNodeFlags_Selected;

	if (gameObject->GetChildrenAmount() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;


	if (ImGui::TreeNodeEx(gameObject->GetName(), flags))
	{
		if (ImGui::IsItemClicked())
			App->scene->selected_object = gameObject;

		ImGui::PushID(id);
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("HIERARCHY", &id, sizeof(int));
			ImGui::Text("Reparent");
			ImGui::EndDragDropSource();
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY"))
			{
				IM_ASSERT(payload->DataSize == sizeof(int));
				int payload_n = *(const int*)payload->Data;

				std::vector<GameObject*> gameObjects = App->scene->GetAllGameObjects();
				GameObject* target = gameObjects[payload_n];
				target->Reparent(gameObject);
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::PopID();
		id++;

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::Button("Add Empty Child"))
			{
				gameObject->AddChild(new GameObject());
				ImGui::CloseCurrentPopup();
			}
			else if (ImGui::Button("Delete"))
			{
				gameObject->to_delete = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		for (size_t i = 0; i < gameObject->GetChildrenAmount(); i++)
		{
			PreorderHierarchy(gameObject->GetChildAt(i), id);
		}
		ImGui::TreePop();
	}
}