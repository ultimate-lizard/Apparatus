#include "Model.h"

Model::Model(const std::string& resourceName, Shader* shader, std::vector<Mesh*>&& meshes, std::vector<Material*>&& materials) :
	Asset(resourceName),
	shader(shader),
	meshes(std::forward<std::vector<Mesh*>>(meshes)),
	materialSlots(std::forward<std::vector<Material*>>(materials))
{

}

void Model::init()
{

}

const std::vector<Mesh*>& Model::getMeshes() const
{
	return meshes;
}

const std::vector<Material*>& Model::getMaterialSlots() const
{
	return materialSlots;
}

Material* Model::getMaterialSlot(size_t index)
{
	if (index < materialSlots.size())
	{
		return materialSlots[index];
	}

	return nullptr;
}
