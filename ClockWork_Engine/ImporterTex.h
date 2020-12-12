
#include "Globals.h"

struct TextureImportingOptions;
class ResourceTex;

typedef unsigned int ILenum;

namespace ImporterTex
{
	void Init();

	void Import(char* fileBuffer, ResourceTex* resource, uint size);
	uint Save(ResourceTex* texture, char** fileBuffer);
	bool Load(char* fileBuffer, ResourceTex* texture, uint size);

	void UnloadTexture(uint imageID);
	ILenum GetFileFormat(const char* file);
	void ApplyImportingOptions(TextureImportingOptions importingOptions);
}


