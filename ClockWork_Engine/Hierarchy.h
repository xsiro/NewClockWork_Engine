#pragma once
#include "Window.h"

class GameObject;

class Hierarchy : public Window {
public:
	Hierarchy();
	~Hierarchy();

	void Draw() override;
	void PreorderHierarchy(GameObject* gameObject, int& id);
};

