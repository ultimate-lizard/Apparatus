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

ModelInstance::ModelInstance(Model* model) :
	model(model)
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
