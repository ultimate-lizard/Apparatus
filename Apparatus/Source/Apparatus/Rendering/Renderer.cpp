#include "Renderer.h"

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "Material.h"
#include "Model.h"
#include "Camera.h"
#include "Shader.h"
#include "Light.h"
#include "Texture.h"
#include "../Window.h"
#include "../Core/Logger.h"
#include "../Server/Entity.h"
#include "../Components/ModelComponent.h"
#include "../Components/TransformComponent.h"

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
}

void Renderer::render()
{
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

				if (command.material)
				{
					if (Material* material = command.material)
					{
						material->submitUniforms();

						if (Shader* shader = material->getShader())
						{
							shader->bind();

							// TODO: Transform Uniform Buffer
							shader->setUniform("world", command.worldMatrix);

							// Camera
							if (command.camera)
							{
								shader->setUniform("view", command.camera->getView());
								shader->setUniform("projection", command.camera->getProjection());
								shader->setUniform("cameraPos", command.camera->getPosition());
							}

							if (UniformBufferObject* ubo = shader->findUniformBufferObject("LightUniformBuffer"))
							{
								const int lightsNum = static_cast<int>(lights.size());

								ubo->bind();
								ubo->copySubData(0, sizeof(int), &lightsNum);

								struct LightData
								{
									glm::vec3 color;
									float compression;

									glm::vec3 position;
									float radius;

									glm::vec3 direction;

									float cutOff;
									float outerCutOff;
									int type;
								};

								int offset = 16;

								for (Light* light : lights)
								{
									if (!light)
									{
										continue;
									}

									LightData data;

									data.color = light->getColor();
									data.position = light->getPosition();

									data.radius = light->getRadius();
									data.compression = light->getCompression();

									data.direction = glm::normalize(light->getDirection());
									data.cutOff = glm::cos(glm::radians(light->getCutOff()));
									data.outerCutOff = glm::cos(glm::radians(light->getOuterCutOff()));

									data.type = light->getType();

									ubo->copySubData(offset, 64, &data);
									offset += 64;
								}

								ubo->unbind();
							}
						}
					}

					// Draw
					std::shared_ptr<VertexBuffer<ModelVertex>> vertexBuffer = command.mesh->getVertexBuffer<VertexBuffer<ModelVertex>>();
					if (!vertexBuffer)
					{
						assert(false);
					}

					const std::vector<unsigned int>& indices = command.mesh->getIndices();

					glPointSize(command.drawSize);
					glLineWidth(command.drawSize);

					// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

					const GLenum mode = static_cast<GLenum>(command.renderMode);

					if (indices.size())
					{
						glDrawElements(mode, static_cast<int>(indices.size()), GL_UNSIGNED_INT, 0);
					}
					else if (vertexBuffer->vertices.size())
					{
						glDrawArrays(mode, 0, static_cast<int>(vertexBuffer->vertices.size()));
					}

					glPointSize(1.0f);
					glLineWidth(1.0f);
				}
			}
		}

		commands.clear();
	}
}

void Renderer::push(Mesh* mesh, Material* material, Camera* camera, const glm::mat4 worldMatrix, RenderMode renderMode, float drawSize, size_t depthBufferLayer)
{
	commandsContainer[depthBufferLayer].push_back({ mesh, material, camera, worldMatrix, renderMode, drawSize, depthBufferLayer });
}

void Renderer::push(Model* model, Camera* camera, const glm::mat4& worldMatrix, size_t depthBufferLayer)
{
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

		command.material = model->getMaterialSlot(mesh->getMaterialIndex());
		command.camera = camera;
		command.worldMatrix = worldMatrix;
		command.renderMode = RenderMode::Triangles;
		command.depthBufferLayer = depthBufferLayer;

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

void Renderer::addLight(Light* light)
{
	lights.push_back(light);
}

void Renderer::removeLight(Light* light)
{
	auto iter = std::find(lights.begin(), lights.end(), light);
	lights.erase(iter);
}
