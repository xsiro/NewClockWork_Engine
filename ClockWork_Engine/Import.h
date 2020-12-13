#ifndef _IMPORT_H_
#define _IMPORT_H_

#include "Window.h"
#include "options.h"
#include "Resource.h"

class Import : public Window {
public:
	Import();
	~Import();

	void Draw() override;
	bool DrawModelImportingWindow();
	bool DrawTextureImportingWindow();

	void Enable(const char* file, ResourceType resourceType);

private:
	const char* _fileToImport;
	Options _modelImportingOptions;
	TextureImportingOptions _textureImportingOptions;
	ResourceType _currentResourceType;
};

#endif