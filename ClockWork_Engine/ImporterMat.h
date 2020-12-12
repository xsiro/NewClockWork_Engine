
#include "Globals.h"
#include <string>
class aiMaterial;
class ResourceMaterial;

namespace ImporterMat
{
	void Import(const aiMaterial* aimaterial, ResourceMaterial* material);
	uint64 Save(ResourceMaterial* material, char** fileBuffer);
	bool Load(const char* fileBuffer, ResourceMaterial* material, uint size);

	std::string FindTexture(const char* texture_name, const char* model_directory);
	bool DeleteTexture(const char* material_library_path);
	const char* ExtractTexture(const char* material_library_path);
}


