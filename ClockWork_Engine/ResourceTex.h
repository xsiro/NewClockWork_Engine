#pragma once
#include "Globals.h"
#include "Resource.h"

typedef unsigned char GLubyte;
class JSON;

class ResourceTex : public Resource {
public:
	ResourceTex(uint UID);
	~ResourceTex();

	void GenerateBuffers();
	void BindTexture();

	void FillData(GLubyte* data, uint id, int width, int height);
	uint SaveMeta(JSON& base_object, uint last_modification) override;
	void Load(JSON& base_object) override;

	uint GetID();
	int GeWidth();
	int GetHeight();
	GLubyte* GetData();
	uint GetGpuID();

private:
	uint _id;
	int _width;
	int _height;
	GLubyte* _data;
	uint _gpu_ID;
};

