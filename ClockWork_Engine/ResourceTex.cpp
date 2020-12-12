#include "ResourceTex.h"
#include "glew/include/glew.h"
#include "Application.h"

ResourceTex::ResourceTex(uint UID) : Resource(UID, ResourceType::RESOURCE_TEXTURE),
_id(0), _width(-1), _height(-1), _data(nullptr), _gpu_ID(0)
{}

ResourceTex::~ResourceTex()
{
	_data = nullptr;
}

void ResourceTex::GenerateBuffers()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &_gpu_ID);
	glBindTexture(GL_TEXTURE_2D, _gpu_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ResourceTex::BindTexture()
{
	glBindTexture(GL_TEXTURE_2D, _gpu_ID);
}

void ResourceTex::FillData(GLubyte* data, uint id, int width, int height)
{
	_data = data;
	_id = id;
	_width = width;
	_height = height;
}

uint ResourceTex::GetID() { return _id; }

int ResourceTex::GeWidth() { return _width; }

int ResourceTex::GetHeight() { return _height; }

GLubyte* ResourceTex::GetData() { return _data; }

uint ResourceTex::GetGpuID() { return _gpu_ID; }

