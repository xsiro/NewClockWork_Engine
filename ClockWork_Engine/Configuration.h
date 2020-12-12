#pragma once
#include "Window.h"
#include <vector>

typedef int GLint;

class Configuration : public Window {
public:
	Configuration();
	~Configuration();

	void Draw() override;

private:
	void GetMemoryStatistics(const char* gpu_brand, GLint& vram_budget, GLint& vram_usage, GLint& vram_available, GLint& vram_reserved);

private:
	std::vector<float> fps_log;
	std::vector<float> ms_log;
};
