#pragma once

#include <string>
#include <memory>
#include <list>

#include <glm/glm.hpp>

#include "Importer.h"
#include "../Rendering/Model.h"

class Shader;
struct aiNode;
struct aiScene;
class Material;

class ModelImporter : public Importer
{
public:
	ModelImporter();
	~ModelImporter() = default;

	ModelImporter(const ModelImporter&) = delete;
	ModelImporter(ModelImporter&&) = delete;
	void operator=(const ModelImporter&) = delete;

	// Imports multiple meshes from a single fbx file as a single model
	std::unique_ptr<Model> import(const std::string& modelName, Shader* shader, const std::string& path);

	// Imports multiple meshes from a single fbx file as multiple models
	std::list<std::unique_ptr<Model>> importMultiple(Shader* shader, const std::string& path);

private:
	void processNode(const aiNode* aiNode, const aiScene* aiScene);
	void processMaterials(const aiScene* aiScene);

	glm::mat4 getTransform(const aiNode* node);

	std::string modelName;
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;
	Shader* shader;
};
