#include "MaterialInstance.h"

MaterialInstance::MaterialInstance() :
	material(nullptr)
{
}

Material* MaterialInstance::getMaterial()
{
    return material;
}

MaterialParameters& MaterialInstance::getMaterialParameters()
{
	return parameters;
}
