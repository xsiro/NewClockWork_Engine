#pragma once
#include "Window.h"

#include <string>

class Assets : public Window {
public:
	Assets();
	~Assets();

	void Draw() override;
	const char* GetFileAt(int i);

private:
	void DrawDirectoryRecursive(const char* directory, const char* filter_extension);
	void DrawCurrentFolder();
	char selectedItem[256];
private:
	std::string current_folder;
};

