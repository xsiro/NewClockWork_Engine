#include "ModuleMaterial.h"
#include "ModuleMesh.h"
#include "imgui.h"
#include "FileSys.h"
#include "GameObject.h"
#include "Application.h"
#include "ResourceTex.h"
#include "glew/include/glew.h"
#include "ResourceMat.h"
#include "Assets.h"

ModuleMaterial::ModuleMaterial() : ModuleComponent(), checkers_image(false), _resource(nullptr), colored(false)
{
	type = ComponentType::MATERIAL;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &checkersID);
	glBindTexture(GL_TEXTURE_2D, checkersID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

ModuleMaterial::ModuleMaterial(GameObject* gameObject) : ModuleComponent(gameObject), checkers_image(false), _resource(nullptr), _diffuseTexture(nullptr)
{
	type = ComponentType::MATERIAL;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &checkersID);
	glBindTexture(GL_TEXTURE_2D, checkersID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

ModuleMaterial::~ModuleMaterial()
{
	if (_resource != nullptr)
	{
		App->res->ReleaseResource(_resourceUID);
		_resource = nullptr;

		if (_diffuseTexture != nullptr)
		{
			App->res->ReleaseResource(_diffuseTexture->GetUID());
			_diffuseTexture = nullptr;
		}
	}
}

void ModuleMaterial::Update() {}

void ModuleMaterial::SetResourceUID(uint UID)
{
	_resourceUID = UID;
	_resource = dynamic_cast<ResourceMat*>(App->res->RequestResource(UID));

	if (_resource->diffuseTextureUID != 0)
	{
		if (_diffuseTexture != nullptr)
			App->res->ReleaseResource(_diffuseTexture->GetUID());

		_diffuseTexture = dynamic_cast<ResourceTex*>(App->res->RequestResource(_resource->diffuseTextureUID));
	}

	if (_diffuseTexture == nullptr)
		AssignCheckersImage();
	else
		SetTexture(_diffuseTexture);
}

void ModuleMaterial::BindTexture()
{
	//TODO: Request resource every frame
	if (!checkers_image)
		_diffuseTexture->BindTexture();
	else
		glBindTexture(GL_TEXTURE_2D, checkersID);
}



void ModuleMaterial::OnEditor()
{
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Checkbox("Enabled", &enabled)) {}

		ImGui::SameLine();
		if (ImGui::Checkbox("Checkers Image", &checkers_image))
		{
			if (checkers_image)
			{
				AssignCheckersImage();
			}
			else
			{
				if (_diffuseTexture != nullptr)
					SetTexture(_diffuseTexture);
				else
					checkers_image = true;
			}
		}

		ImGui::Separator();

		if (_diffuseTexture != nullptr && checkers_image == false)
		{
			ImGui::Text("Texture: %s", _diffuseTexture->assetsFile.c_str());
			ImGui::Text("Width: %d Height: %d", _diffuseTexture->GeWidth(), _diffuseTexture->GetHeight());

			ImGui::Spacing();

			ImGui::Image((ImTextureID)_diffuseTexture->GetGpuID(), ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS"))
				{
					IM_ASSERT(payload->DataSize == sizeof(int));
					int payload_n = *(const int*)payload->Data;
					Assets* assets_window = (Assets*)App->gui->windows[ASSETS_WINDOW];
					const char* file = assets_window->GetFileAt(payload_n);
					Resource* possible_texture = App->res->RequestResource(App->res->Find(file));

					if (possible_texture->GetType() == ResourceType::RESOURCE_TEXTURE)
						_diffuseTexture = (ResourceTex*)possible_texture;
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::SameLine();
			if (ImGui::Button("Remove Texture"))
			{
				_diffuseTexture = nullptr;
				AssignCheckersImage();
			}
		}
		else
		{
			ImGui::Image((ImTextureID)checkersID, ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETS"))
				{
					IM_ASSERT(payload->DataSize == sizeof(int));
					int payload_n = *(const int*)payload->Data;
					Assets* assets_window = (Assets*)App->gui->windows[ASSETS_WINDOW];
					const char* file = assets_window->GetFileAt(payload_n);
					Resource* possible_texture = App->res->RequestResource(App->res->Find(file));

					if (possible_texture->GetType() == ResourceType::RESOURCE_TEXTURE)
						SetTexture(dynamic_cast<ResourceTex*>(possible_texture));
				}
				ImGui::EndDragDropTarget();
			}
		}
	}
}

void ModuleMaterial::SetTexture(ResourceTex* texture)
{
	_diffuseTexture = texture;
	checkers_image = false;
}

void ModuleMaterial::AssignCheckersImage()
{
	int CHECKERS_WIDTH = 64;
	int CHECKERS_HEIGHT = 64;

	GLubyte checkerImage[64][64][4];

	for (int i = 0; i < CHECKERS_HEIGHT; i++) {
		for (int j = 0; j < CHECKERS_WIDTH; j++) {
			int c = ((((i & 0x4) == 0) ^ (((j & 0x4)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}

	glBindTexture(GL_TEXTURE_2D, checkersID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
	glBindTexture(GL_TEXTURE_2D, 0);

	checkers_image = true;
}

ResourceTex* ModuleMaterial::GetDiffuseTexture()
{
	return _diffuseTexture;
}
