#pragma once

#include "Window.h"

class Scene : public Window {
public:
	Scene();
	~Scene();

	void Draw() override;
	void DrawGameTimeDataOverlay();

};

