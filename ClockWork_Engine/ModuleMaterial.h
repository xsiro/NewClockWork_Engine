#pragma once
#include "ModuleComponent.h"
#include "Globals.h"

#include <string>

class GnMesh;
class ResourceMat;
class ResourceTex;
class GnJSONArray;

class ModuleMaterial : public ModuleComponent {
public:
	ModuleMaterial();
	ModuleMaterial(GameObject* gameObject);
	~ModuleMaterial();

	void Update() override;
	void SetResourceUID(uint UID) override;

	void BindTexture();

	void Save(GnJSONArray& save_array) override;
	void Load(JSON& load_object) override;
	void OnEditor() override;

	void SetTexture(ResourceTex* texture);
	void AssignCheckersImage();
	ResourceTex* GetDiffuseTexture();

private:
	bool checkers_image;
	bool colored;

	ResourceMat* _resource;
	ResourceTex* _diffuseTexture;
	uint checkersID;
};