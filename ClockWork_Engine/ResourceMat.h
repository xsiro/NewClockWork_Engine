#pragma once
#include "Globals.h"
#include "Resource.h"
#include "Color.h"

class ResourceMat : public Resource {
public:
	ResourceMat(uint UID);
	~ResourceMat();

public:
	uint diffuseTextureUID;
	Color diffuseColor;
};

