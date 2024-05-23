#include "ModelImporter.h"

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>

#include "../Rendering/Mesh.h"
#include "../Rendering/Texture.h"
#include "../Rendering/Material.h"
#include "Logger.h"
#include "AssetManager.h"
#include "TextureImporter.h"

ModelImporter::ModelImporter() :
	shader(nullptr)
{
}

std::unique_ptr<Model> ModelImporter::import(Shader * shader, const std::string & path)
{
	meshes.clear();
	materials.clear();

	this->modelName = modelName;
	this->shader = shader;

	Assimp::Importer import;
	const aiScene * aiScene = import.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GlobalScale | aiProcess_GenSmoothNormals);

	if (!aiScene)
	{
		LOG("Unable to import model '" + modelName + "' because aiScene was NULL", LogLevel::Error);
		return nullptr;
	}

	processNode(aiScene->mRootNode, aiScene);
	processMaterials(aiScene);

	return std::make_unique<Model>(shader, std::move(meshes), std::move(materials));
}

std::list<std::unique_ptr<Model>> ModelImporter::importMultiple(Shader* shader, const std::string& path)
{
	modelName.clear();
	meshes.clear();
	materials.clear();

	std::list<std::unique_ptr<Model>> result;

	Assimp::Importer import;
	const aiScene * aiScene = import.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GlobalScale);

	if (!aiScene)
	{
		LOG("Unable to import multiple meshes because aiScene was NULL", LogLevel::Error);
		return result;
	}

	processNode(aiScene->mRootNode, aiScene);
	processMaterials(aiScene);

	for (Mesh* mesh : meshes)
	{
		if (mesh)
		{
			size_t materialIndex = mesh->getMaterialIndex();
			if (materialIndex < materials.size())
			{
				if (Material* material = materials[materialIndex])
				{
					std::vector<Mesh*> meshInVector;
					meshInVector.push_back(mesh);

					std::vector<Material*> materialInVector;
					materialInVector.push_back(material);

					mesh->materialIndex = 0;
					auto newModel = std::make_unique<Model>(shader, std::move(meshInVector), std::move(materialInVector));
					newModel->setAssetName("Model_" + mesh->getAssetName());
					result.push_back(std::move(newModel));
				}
			}
		}
	}

	return result;
}

void ModelImporter::processNode(const aiNode* aiNode, const aiScene* aiScene)
{
	if (!aiNode || !aiScene)
	{
		return;
	}

	for (unsigned int i = 0; i < aiNode->mNumMeshes; ++i)
	{
		glm::mat4 transform = getTransform(aiNode);
		if (aiNode->mParent)
		{
			transform *= getTransform(aiNode->mParent);
		}

		std::vector<ModelVertex> vertices;
		std::vector<unsigned int> indices;

		if (aiMesh* aiMesh = aiScene->mMeshes[aiNode->mMeshes[i]])
		{
			aiString meshName = aiNode->mName;

			for (unsigned int i = 0; i < aiMesh->mNumVertices; ++i)
			{
				aiVector3D& aiVec = aiMesh->mVertices[i];

				ModelVertex vertex;
				vertex.position = glm::vec3(aiVec.x, aiVec.y, aiVec.z);

				glm::vec4 newPosition = glm::vec4(vertex.position, 1.0f);
				newPosition = newPosition * transform;
				vertex.position = newPosition;

				if (aiVector3D* aiUvArray = aiMesh->mTextureCoords[0])
				{
					vertex.uv = glm::vec2(aiUvArray[i].x, aiUvArray[i].y);
				}

				if (aiMesh->HasNormals())
				{
					aiVector3D aiNormal = aiMesh->mNormals[i];

					vertex.normal.x = aiNormal.x;
					vertex.normal.y = aiNormal.y;
					vertex.normal.z = aiNormal.z;

					glm::vec4 newNormal = glm::vec4(vertex.normal, 0.0f);
					newNormal = newNormal * transform;
					vertex.normal = glm::normalize(newNormal);
				}

				vertices.push_back(std::move(vertex));
			}

			for (unsigned int i = 0; i < aiMesh->mNumFaces; ++i)
			{
				aiFace& face = aiMesh->mFaces[i];

				for (unsigned int j = 0; j < face.mNumIndices; ++j)
				{
					indices.push_back(face.mIndices[j]);
				}
			}

			unsigned int materialIndex = aiMesh->mMaterialIndex;

			std::shared_ptr<VertexBuffer<ModelVertex>> vertexBuffer = std::make_shared<VertexBuffer<ModelVertex>>();
			vertexBuffer->vertices = vertices;

			auto vao = std::make_unique<VertexArrayObject>();
			assert(vao);
			vao->setStride(sizeof(ModelVertex));
			vao->addAttribute(3);
			vao->addAttribute(2);
			vao->addAttribute(3);
			vao->addAttribute(4);

			if (Mesh* mesh = assetManager->createAsset<Mesh>(meshName.C_Str(), std::move(vao), vertexBuffer, indices, materialIndex))
			{
				meshes.push_back(mesh);
			}
		}
	}

	for (unsigned int j = 0; j < aiNode->mNumChildren; ++j)
	{
		processNode(aiNode->mChildren[j], aiScene);
	}
}

void ModelImporter::processMaterials(const aiScene* aiScene)
{
	for (unsigned int i = 0; i < aiScene->mNumMaterials; ++i)
	{
		aiMaterial* aiMaterial = aiScene->mMaterials[i];
		if (!aiMaterial)
		{
			continue;
		}

		aiString materialName = aiMaterial->GetName();
		if (Material* newMaterial = assetManager->createAsset<Material>(std::string(materialName.C_Str())))
		{
			newMaterial->setShader(shader);
			newMaterial->createBoolParameter("material.selected", false);
			newMaterial->createVec4Parameter("material.selectionColor", { 1.0f, 0.0f, 0.0f, 1.0f });

			aiColor3D diffuseColor;
			aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
			newMaterial->createVec3Parameter("material.color", { diffuseColor.r, diffuseColor.g, diffuseColor.b });

			aiColor3D specularColor;
			aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);

			float shininess = 0.0f;
			aiMaterial->Get(AI_MATKEY_SHININESS, shininess);

			float shininessStrength = 1.0f;
			aiMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength);

			// TODO: This will be replaced by the specular map in the future
			newMaterial->createVec3Parameter("material.specular", glm::vec3(specularColor.r * shininessStrength, specularColor.g * shininessStrength, specularColor.b * shininessStrength));

			newMaterial->createFloatParameter("material.shininess", shininess);

			for (unsigned int i = 0; i < aiMaterial->GetTextureCount(aiTextureType_DIFFUSE); ++i)
			{
				aiString texturePath;
				aiMaterial->GetTexture(aiTextureType_DIFFUSE, i, &texturePath);

				if (TextureImporter* textureImporter = assetManager->getImporter<TextureImporter>())
				{
					std::string textureName = "Texture_";
					textureName += materialName.C_Str();
					if (std::unique_ptr<Texture> newTexturePtr = textureImporter->import("../Models/" + std::string(texturePath.C_Str())))
					{
						if (Texture* newTexture = assetManager->createAsset<Texture>(textureName, std::move(newTexturePtr)))
						{
							newMaterial->createTextureParameter("material.diffuseTexture", newTexture);
						}
					}
				}
			}

			materials.push_back(newMaterial);
		}
	}
}

glm::mat4 ModelImporter::getTransform(const aiNode* node)
{
	if (!node)
	{
		return glm::mat4(1.0f);
	}

	aiMatrix4x4 aiTransform = node->mTransformation;

	glm::mat4 transform;
	transform[0][0] = aiTransform.a1;
	transform[0][1] = aiTransform.a2;
	transform[0][2] = aiTransform.a3;
	transform[0][3] = aiTransform.a4;

	transform[1][0] = aiTransform.b1;
	transform[1][1] = aiTransform.b2;
	transform[1][2] = aiTransform.b3;
	transform[1][3] = aiTransform.b4;

	transform[2][0] = aiTransform.c1;
	transform[2][1] = aiTransform.c2;
	transform[2][2] = aiTransform.c3;
	transform[2][3] = aiTransform.c4;

	transform[3][0] = aiTransform.d1;
	transform[3][1] = aiTransform.d2;
	transform[3][2] = aiTransform.d3;
	transform[3][3] = aiTransform.d4;

	return transform;
}
