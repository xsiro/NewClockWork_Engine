
#pragma once
#include "Color.h"
#include "glmath.h"
#include "ModuleComponent.h"


class GameObject;

class Light : public ModuleComponent {
public:
	Light();
	Light(GameObject* gameObject);
	~Light();

	void Init();
	void Update() override;
	void SetPos(float x, float y, float z);
	void Active(bool active);
	void Render();
	void OnEditor() override;
	

public:
	vec3 position;
	Color ambient;
	Color diffuse;

	int ref;
};