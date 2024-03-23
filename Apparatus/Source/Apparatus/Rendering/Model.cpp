#include "Model.h"

Model::Model(const std::string& resourceName, Shader* shader, std::vector<Mesh*>&& meshes, std::vector<Material*>&& materials) :
	Asset(resourceName),
	shader(shader),
	meshes(std::forward<std::vector<Mesh*>>(meshes)),
	materials(std::forward<std::vector<Material*>>(materials))
{

}

void Model::init()
{

}

std::unique_ptr<ModelInstance> Model::createModelInstance()
{
	return std::make_unique<ModelInstance>(this);
}

const std::vector<Mesh*>& Model::getMeshes() const
{
	return meshes;
}

const std::vector<Material*>& Model::getMaterials() const
{
	return materials;
}
