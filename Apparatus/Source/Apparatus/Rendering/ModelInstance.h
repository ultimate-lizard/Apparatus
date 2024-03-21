#pragma once

#include "Material.h"

class Model;

class ModelInstance
{
public:
	ModelInstance(Model* model);
	ModelInstance(ModelInstance&& other) = delete;
	ModelInstance(const ModelInstance& other) = delete;
	void operator=(const ModelInstance& other) = delete;

	Model* getModel();
	MaterialInstance* getMaterialInstance(size_t materialSlot);

private:
	Model* model;
	std::vector<std::unique_ptr<MaterialInstance>> materialInstances;

	friend class Model;
};
