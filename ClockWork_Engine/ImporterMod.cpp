#include "ImporterMod.h"
#include "Application.h"
#include "FileSys.h"
#include <unordered_set>

#include "GameObject.h"
#include "ModuleTransform.h"
#include "ModuleMesh.h"
#include "ModuleMaterial.h"
#include "Cam.h"

#include "ResourceMod.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#pragma comment (lib, "Assimp/Assimp/libx86/assimp.lib")

void ImporterMod::Import(char* fileBuffer, ResourceMod* model, uint size)
{
	Timer timer;
	timer.Start();

	const aiScene* scene = nullptr;
	scene = aiImportFileFromMemory(fileBuffer, size, aiProcessPreset_TargetRealtime_MaxQuality, NULL);

	if (scene != nullptr && scene->HasMeshes())
	{
		for (size_t i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* aimesh = scene->mMeshes[i];
			model->meshes.push_back(App->res->ImportInternalResource(model->assetsFile.c_str(), aimesh, ResourceType::RESOURCE_MESH));
		}

		for (size_t i = 0; i < scene->mNumMaterials; i++)
		{
			aiMaterial* aimaterial = scene->mMaterials[i];
			model->materials.push_back(App->res->ImportInternalResource(model->assetsFile.c_str(), aimaterial, ResourceType::RESOURCE_MATERIAL));
		}

		if (!App->res->modelImportingOptions.ignoreLights)
		{
			for (size_t i = 0; i < scene->mNumLights; i++)
			{
				Light* light = new Light();
				aiLight* ai_light = scene->mLights[i];
				light->name = ai_light->mName.C_Str();
				light->SetPos(ai_light->mPosition.x, ai_light->mPosition.y, ai_light->mPosition.z);
				light->ambient = Color(ai_light->mColorAmbient.r, ai_light->mColorAmbient.g, ai_light->mColorAmbient.b);
				light->diffuse = Color(ai_light->mColorDiffuse.r, ai_light->mColorDiffuse.g, ai_light->mColorDiffuse.b);
				model->lights.push_back(light);
			}
		}

		if (!App->res->modelImportingOptions.ignoreCameras)
		{
			for (size_t i = 0; i < scene->mNumCameras; i++)
			{
				Camera* camera = new Camera;
				aiCamera* aicamera = scene->mCameras[i];
				camera->name = aicamera->mName.C_Str();
				camera->SetHorizontalFieldOfView(aicamera->mHorizontalFOV);
				camera->SetPosition(float3(aicamera->mPosition.x, aicamera->mPosition.y, aicamera->mPosition.z));
				camera->SetNearPlaneDistance(aicamera->mClipPlaneNear);
				camera->SetFarPlaneDistance(aicamera->mClipPlaneFar);
				camera->SetReference(float3(aicamera->mLookAt.x, aicamera->mLookAt.y, aicamera->mLookAt.z));
				model->cameras.push_back(camera);
			}
		}

		aiNode* rootNode = scene->mRootNode;
		ImportChildren(scene, rootNode, nullptr, nullptr, model);
		ConvertToDesiredAxis(rootNode, model->nodes[0]);
		aiReleaseImport(scene);

		LOG("%s: imported in %d ms", model->assetsFile.c_str(), timer.Read());
	}
	else
		LOG_ERROR("Error importing: %s", model->assetsFile.c_str());
}

void ImporterMod::ImportChildren(const aiScene* scene, aiNode* ainode, aiNode* parentAiNode, ModelNode* parentNode, ResourceMod* model)
{
	ModelNode modelNode;

	if (ainode == scene->mRootNode)
	{
		modelNode.name = FileSys::GetFileName(model->assetsFile.c_str());
		modelNode.parentUID = 0;
	}
	else
	{
		modelNode.name = ainode->mName.C_Str();
		modelNode.parentUID = parentNode->UID;
	}

	modelNode.UID = App->res->GenerateUID();

	LoadTransform(ainode, modelNode);

	if (modelNode.name.find("_$AssimpFbx$_") != std::string::npos && ainode->mNumChildren == 1)
	{
		modelNode.UID = parentNode->UID;
		modelNode.parentUID = parentNode->parentUID;
		AddParentTransform(&modelNode, parentNode);
	}
	else
	{
		if (parentNode != nullptr && parentNode->name.find("_$AssimpFbx$_") != std::string::npos)
		{
			AddParentTransform(&modelNode, parentNode);
		}

		if (ainode->mMeshes != nullptr)
		{
			//Mesh --------------------------------------------------------------
			modelNode.meshID = model->meshes[*ainode->mMeshes];

			//Materials ----------------------------------------------------------
			aiMesh* aimesh = scene->mMeshes[*ainode->mMeshes];
			modelNode.materialID = model->materials[aimesh->mMaterialIndex];
		}

		model->nodes.push_back(modelNode);
	}

	for (size_t i = 0; i < ainode->mNumChildren; i++)
	{
		ImportChildren(scene, ainode->mChildren[i], ainode, &modelNode, model);
	}
}

void ImporterMod::AddParentTransform(ModelNode* node, ModelNode* parentNode)
{
	node->position += parentNode->position;
	node->rotation = node->rotation.Mul(parentNode->rotation);
	node->scale = node->scale.Mul(parentNode->scale);
}

void ImporterMod::ReimportFile(char* fileBuffer, ResourceMod* newModel, uint size)
{
	std::string meta_file = newModel->assetsFile;
	meta_file.append(".meta");

	ResourceMod oldModel(0);
	ExtractInternalResources(meta_file.c_str(), oldModel);

	Import(fileBuffer, newModel, size);

	for (size_t n = 0; n < newModel->nodes.size(); n++)
	{
		for (size_t o = 0; o < oldModel.nodes.size(); o++)
		{
			if (oldModel.nodes[o].name == newModel->nodes[n].name)
			{
				newModel->nodes[n].meshID = oldModel.nodes[o].meshID;
				std::string mesh_path = App->res->GenerateLibraryPath(oldModel.nodes[o].meshID, ResourceType::RESOURCE_MESH);
				if (FileSys::Exists(mesh_path.c_str()))
				{
					std::string temp_path = App->res->GenerateLibraryPath(newModel->nodes[n].meshID, ResourceType::RESOURCE_MESH);
					FileSys::Rename(temp_path.c_str(), mesh_path.c_str());
				}

				newModel->nodes[n].materialID = oldModel.nodes[o].materialID;
				std::string material_path = App->res->GenerateLibraryPath(oldModel.nodes[o].materialID, ResourceType::RESOURCE_MATERIAL);
				if (FileSys::Exists(material_path.c_str()))
				{
					std::string temp_path = App->res->GenerateLibraryPath(newModel->nodes[n].materialID, ResourceType::RESOURCE_MATERIAL);
					FileSys::Rename(temp_path.c_str(), material_path.c_str());
				}
			}
		}
	}
}

void ImporterMod::LoadTransform(aiNode* node, ModelNode& modelNode)
{
	aiVector3D position, scaling;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scaling, rotation, position);
	//eulerRotation = rotation.GetEuler() * RADTODEG;

	if (App->res->modelImportingOptions.normalizeScales && scaling.x == 100 && scaling.y == 100 && scaling.z == 100) {
		scaling.x = scaling.y = scaling.z = 1.0f;
	}

	scaling *= App->res->modelImportingOptions.globalScale;

	modelNode.position = float3(position.x, position.y, position.z);
	modelNode.rotation = Quat(rotation.x, rotation.y, rotation.z, rotation.w);
	modelNode.scale = float3(scaling.x, scaling.y, scaling.z);
}



GameObject* ImporterMod::ConvertToGameObject(ResourceMod* model)
{
	if (model == nullptr) {
		LOG_ERROR("Trying to load null model");
		return nullptr;
	}

	std::vector<GameObject*> createdGameObjects;

	GameObject* root = nullptr;

	for (size_t i = 0; i < model->nodes.size(); i++)
	{
		GameObject* gameObject = new GameObject();
		gameObject->SetName(model->nodes[i].name.c_str());
		gameObject->UUID = model->nodes[i].UID;
		gameObject->GetTransform()->SetPosition(model->nodes[i].position);
		gameObject->GetTransform()->SetRotation(model->nodes[i].rotation);
		gameObject->GetTransform()->SetScale(model->nodes[i].scale);

		if (model->nodes[i].meshID != -1)
		{
			GnMesh* mesh = (GnMesh*)gameObject->AddComponent(ComponentType::MESH);
			mesh->SetResourceUID(model->nodes[i].meshID);
		}

		if (model->nodes[i].materialID != -1)
		{
			ModuleMaterial* material = (ModuleMaterial*)gameObject->AddComponent(ComponentType::MATERIAL);
			material->SetResourceUID(model->nodes[i].materialID);
		}

		if (model->nodes[i].parentUID == 0)
			root = gameObject;

		for (size_t j = 0; j < createdGameObjects.size(); j++)
		{
			if (createdGameObjects[j]->UUID == model->nodes[i].parentUID)
			{
				createdGameObjects[j]->AddChild(gameObject);
				gameObject->SetParent(createdGameObjects[j]);
			}
		}

		createdGameObjects.push_back(gameObject);
	}

	for (size_t i = 0; i < model->lights.size(); i++) {
		GameObject* light = new GameObject();
		light->SetName(model->lights[i]->name.c_str());
		light->AddComponent(model->lights[i]);
		root->AddChild(light);
	}

	for (size_t i = 0; i < model->cameras.size(); i++) {
		GameObject* camera = new GameObject();
		camera->SetName(model->cameras[i]->name.c_str());
		camera->AddComponent(model->cameras[i]);
		root->AddChild(camera);
	}

	App->res->ReleaseResource(model->GetUID());
	root->UpdateChildrenTransforms();

	return root;
}



void ImporterMod::ConvertToDesiredAxis(aiNode* node, ModelNode& modelNode)
{
	/*
		if (node->mMetaData == nullptr)
		return;

	int upAxis;
	node->mMetaData->Get("UpAxis", upAxis);
	int upAxisSign;
	node->mMetaData->Get("UpAxisSign", upAxisSign);
	upAxis *= upAxisSign;

	int frontAxis;
	node->mMetaData->Get("frontAxis", frontAxis);
	int frontAxisSign;
	node->mMetaData->Get("frontAxisSign", frontAxisSign);
	frontAxis *= frontAxisSign;

	int coordAxis;
	node->mMetaData->Get("coordAxis", coordAxis);
	int coordAxisSign;
	node->mMetaData->Get("coordAxisSign", coordAxisSign);
	coordAxis *= coordAxisSign;

	float3x3 modelBasis = float3x3::zero;
	modelBasis[coordAxis][0] = coordAxisSign;
	modelBasis[upAxis][1] = upAxisSign;
	modelBasis[frontAxis][2] = frontAxisSign;

	float3x3 desiredBasis = float3x3::zero;

	float3 desiredForwardAxis = float3::zero;
	desiredForwardAxis[App->resources->modelImportingOptions.forwardAxis % 3] = App->resources->modelImportingOptions.forwardAxis;

	float3 desiredUpAxis = float3::zero;
	desiredUpAxis[App->resources->modelImportingOptions.upAxis % 3] = App->resources->modelImportingOptions.upAxis;

	float3 desiredCoordAxis = float3::zero;
	desiredCoordAxis = desiredForwardAxis.Cross(desiredUpAxis);

	desiredBasis.SetCol(0, desiredCoordAxis);
	desiredBasis.SetCol(1, desiredUpAxis);
	desiredBasis.SetCol(2, desiredForwardAxis);

	float3 rotation = modelBasis.ToEulerXYZ();

	modelNode.rotation = Quat::FromEulerXYZ(rotation.x, rotation.y, rotation.z);
	*/
	modelNode;
}