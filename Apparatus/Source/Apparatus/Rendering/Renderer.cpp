#include "Renderer.h"

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "Material.h"
#include "Model.h"
#include "Camera.h"
#include "Shader.h"
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "../Components/ModelComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/LightComponent/PointLightComponent.h"
#include "../Server/Entity.h"
#include "../Core/Logger.h"
#include "../Window.h"

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	LOG("GL CALLBACK: type = " + std::to_string(type) + " severity = " + std::to_string(severity) + " message = " + message, type == GL_DEBUG_TYPE_ERROR ? LogLevel::Error : LogLevel::Info);
}

Renderer::Renderer(Window& window) :
	window(window),
	context(nullptr),
	uboPointLight(-1),
	lightsIndex(-1),
	activeCamera(nullptr)
{
	commandsContainer.resize(8);
}

void Renderer::init()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

#ifdef _DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	context = SDL_GL_CreateContext(window.getSdlWindow());
	if (!context)
	{
		LOG("Couldn't initialize GL context!", LogLevel::Error);
	}

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		LOG("Couldn't load GL loader!", LogLevel::Error);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
}

void Renderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (!commandsContainer.size())
	{
		return;
	}

	for (std::vector<RenderCommand>& commands : commandsContainer)
	{
		glClear(GL_DEPTH_BUFFER_BIT);

		for (RenderCommand& command : commands)
		{
			if (command.mesh)
			{
				command.mesh->bind();

				if (command.materialInstance)
				{
					if (Material* material = command.materialInstance->getMaterial())
					{
						if (Shader* shader = material->getShader())
						{
							material->bind();
							shader->bind();

							// TODO: Transform Uniform Buffer
							shader->setUniform("world", command.worldMatrix);

							// Camera
							if (command.camera)
							{
								shader->setUniform("view", command.camera->getView());
								shader->setUniform("projection", command.camera->getProjection());
								shader->setUniform("cameraPos", command.camera->getWorldPosition());
							}

							MaterialParameters& params = command.materialInstance->getMaterialParameters();

							for (auto mapIter : params.getAllBoolParameters())
							{
								shader->setUniform("material." + mapIter.first, mapIter.second);
							}

							for (auto mapIter : params.getAllFloatParameters())
							{
								shader->setUniform("material." + mapIter.first, mapIter.second);
							}

							for (auto mapIter : params.getAllVec3Parameters())
							{
								shader->setUniform("material." + mapIter.first, mapIter.second);
							}

							for (auto mapIter : params.getAllVec4Parameters())
							{
								shader->setUniform("material." + mapIter.first, mapIter.second);
							}

							//TODO: Optimize so there is no excess binds
							if (UniformBufferObject* ubo = shader->findUniformBufferObject("LightUniformBuffer"))
							{
								const int lightsNum = static_cast<int>(pointLights.size());

								ubo->bind();
								ubo->copySubData(0, sizeof(int), &lightsNum);

								int offset = 16;

								for (PointLight* pointLight : pointLights)
								{
									if (!pointLight)
									{
										continue;
									}

									struct PointLightData
									{
										glm::vec3 color;
										float compression;
										glm::vec3 position;
										float radius;
									};

									PointLightData data;
									data.color = pointLight->getColor();
									data.compression = pointLight->getCompression();
									data.position = pointLight->getPosition();
									data.radius = pointLight->getRadius();

									ubo->copySubData(offset, sizeof(PointLightData), &data);
									offset += sizeof(PointLightData);
								}

								ubo->unbind();
							}
						}
					}

					// Draw
					const std::vector<Vertex>& vertices = command.mesh->getVertices();
					const std::vector<unsigned int>& indices = command.mesh->getIndices();

					glPointSize(command.drawSize);
					glLineWidth(command.drawSize);

					const GLenum mode = static_cast<GLenum>(command.renderMode);

					if (indices.size())
					{
						glDrawElements(mode, static_cast<int>(indices.size()), GL_UNSIGNED_INT, 0);
					}
					else if (vertices.size())
					{
						glDrawArrays(mode, 0, static_cast<int>(vertices.size()));
					}

					glPointSize(1.0f);
					glLineWidth(1.0f);
				}
			}
		}

		commands.clear();
	}
}

void Renderer::push(Mesh* mesh, MaterialInstance* materialInstance, Camera* camera, const glm::mat4 worldMatrix, RenderMode renderMode, float drawSize, size_t depthBufferLayer)
{
	commandsContainer[depthBufferLayer].push_back({ mesh, materialInstance, camera, worldMatrix, renderMode, drawSize, depthBufferLayer });
}

void Renderer::push(ModelInstance* modelInstance, Camera* camera, const glm::mat4& worldMatrix)
{
	if (!modelInstance)
	{
		return;
	}

	Model* model = modelInstance->getModel();

	if (!model || !camera)
	{
		return;
	}
	
	const std::vector<Mesh*>& meshes = model->getMeshes();

	for (Mesh* mesh : meshes)
	{
		if (!mesh)
		{
			continue;
		}

		RenderCommand command;
		command.mesh = mesh;

		command.materialInstance = modelInstance->getMaterialInstance(mesh->getMaterialIndex());
		command.camera = camera;
		command.worldMatrix = worldMatrix;
		command.renderMode = RenderMode::Triangles;
		command.depthBufferLayer = modelInstance->getDepthBufferLayer();

		commandsContainer[command.depthBufferLayer].push_back(command);
	}
}

void Renderer::setActiveCamera(Camera* camera)
{
	this->activeCamera = camera;
}

Camera* Renderer::getActiveCamera()
{
	return activeCamera;
}

size_t Renderer::getMaxDepthBufferLayer()
{
	return 7;
}

void Renderer::addPointLight(PointLight* pointLight)
{
	pointLights.push_back(pointLight);
}

void Renderer::removeLight(PointLight* pointLight)
{
	auto iter = std::find(pointLights.begin(), pointLights.end(), pointLight);
	pointLights.erase(iter);
}
