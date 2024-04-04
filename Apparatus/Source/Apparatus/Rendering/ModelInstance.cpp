#include "ModelInstance.h"

#include "Model.h"

Model* ModelInstance::getModel()
{
	return model;
}

MaterialInstance* ModelInstance::getMaterialInstance(size_t materialSlot)
{
	if (materialSlot < materialInstances.size())
	{
		return materialInstances[materialSlot].get();
	}

	return nullptr;
}

void ModelInstance::setDepthBufferLayer(size_t layer)
{
	depthBufferLayer = layer;
}

size_t ModelInstance::getDepthBufferLayer() const
{
	return depthBufferLayer;
}


ModelInstance::ModelInstance(Model* model) :
	model(model),
	depthBufferLayer(0)
{
	if (model)
	{
		for (Material* material : model->getMaterials())
		{
			if (material)
			{
				materialInstances.push_back(std::move(material->createMaterialInstance()));
			}
		}
	}
}
