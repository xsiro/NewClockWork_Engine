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

private:
	std::string current_folder;
};

