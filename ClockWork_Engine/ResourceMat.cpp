#include "ResourceMat.h"

ResourceMat::ResourceMat(uint UID) : Resource(UID, ResourceType::RESOURCE_MATERIAL), diffuseTextureUID(0) {}

ResourceMat::~ResourceMat() {}
