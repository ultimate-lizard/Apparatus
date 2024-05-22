#pragma once

#include <string>
#include <vector>
#include <memory>

#include "../Core/Asset.h"

class Shader;
class Mesh;
class Material;

class Model : public Asset
{
public:
	Model(Shader* shader, std::vector<Mesh*>&& meshes, std::vector<Material*>&& materials);

	Model() = delete;
	Model(const Model&) = delete;
	Model(Model&&) = delete;
	void operator=(const Model&) = delete;

	virtual void init() override;

	const std::vector<Mesh*>& getMeshes() const;
	const std::vector<Material*>& getMaterialSlots() const;

	Material* getMaterialSlot(size_t index);

private:
	Shader* shader;
	std::vector<Mesh*> meshes;
	std::vector<Material*> materialSlots;
};
