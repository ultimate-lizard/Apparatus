#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../Core/Resource.h"
#include "ModelInstance.h"

class Shader;
class Mesh;

class Model : public Resource
{
public:
	Model(const std::string& resourceName, Shader* shader, std::vector<Mesh*>&& meshes, std::vector<Material*>&& materials);

	Model() = delete;
	Model(const Model&) = delete;
	Model(Model&&) = delete;
	void operator=(const Model&) = delete;

	virtual void init() override;

	std::unique_ptr<ModelInstance> createModelInstance();

	const std::vector<Mesh*>& getMeshes() const;
	const std::vector<Material*>& getMaterials() const;

private:
	Shader* shader;
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;
};
