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

	void setDepthBufferLayer(size_t layer);
	size_t getDepthBufferLayer() const;

private:
	Model* model;
	std::vector<std::unique_ptr<MaterialInstance>> materialInstances;

	size_t depthBufferLayer;

	friend class Model;
};
