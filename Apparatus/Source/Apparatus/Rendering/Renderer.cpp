#include "Renderer.h"

#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Core/Logger.h"
#include "Mesh.h"
#include "Material.h"
#include "Model.h"
#include "Camera.h"
#include "Shader.h"
#include "Light.h"
#include "../Components/ModelComponent.h"

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	//fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
	//	(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
	//	type, severity, message);

	LOG("GL CALLBACK: type = " + std::to_string(type) + " severity = " + std::to_string(severity) + " message = " + message, type == GL_DEBUG_TYPE_ERROR ? LogLevel::Error : LogLevel::Info);
}

Renderer::Renderer(SDL_Window* window) :
	window(window),
	context(nullptr)
{
	commandsArray.resize(8);
}

void Renderer::init()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	
	context = SDL_GL_CreateContext(window);
	if (!context)
	{
		LOG("Couldn't initialize GL context!", LogLevel::Error);
	}

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		LOG("Couldn't load GL loader!", LogLevel::Error);
	}

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
}

void Renderer::render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	if (!commandsArray.size())
	{
		return;
	}

	for (std::queue<RenderCommand>& commands : commandsArray)
	{
		glClear(GL_DEPTH_BUFFER_BIT);

		while (!commands.empty())
		{
			RenderCommand command = commands.front();
			commands.pop();

			if (command.mesh)
			{
				command.mesh->bind();

				// Material
				if (command.materialInstance)
				{
					if (Material* material = command.materialInstance->getMaterial())
					{
						if (Shader* shader = material->getShader())
						{
							material->bind();
							shader->bind();

							shader->setUniform("world", command.worldMatrix);
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

							// Directional lights
							shader->setUniform("directionalLightNum", static_cast<int>(command.lightingInfo.directionalLights.size()));
							for (size_t i = 0; i < command.lightingInfo.directionalLights.size(); ++i)
							{
								DirectionalLight* light = command.lightingInfo.directionalLights[i];
								if (!light)
								{
									continue;
								}

								shader->setUniform("directionalLight[" + std::to_string(i) + "].direction", light->getDirection());

								shader->setUniform("directionalLight[" + std::to_string(i) + "].light.ambient", light->getAmbientColor());
								shader->setUniform("directionalLight[" + std::to_string(i) + "].light.diffuse", light->getDiffuseColor());
								shader->setUniform("directionalLight[" + std::to_string(i) + "].light.specular", light->getSpecularColor());
							}

							// Point lights
							shader->setUniform("pointLightNum", static_cast<int>(command.lightingInfo.pointLights.size()));
							for (size_t i = 0; i < command.lightingInfo.pointLights.size(); ++i)
							{
								PointLight* light = command.lightingInfo.pointLights[i];
								if (!light)
								{
									continue;
								}

								shader->setUniform("pointLight[" + std::to_string(i) + "].position", light->getPosition());
								
								shader->setUniform("pointLight[" + std::to_string(i) + "].constant", light->getConstant());
								shader->setUniform("pointLight[" + std::to_string(i) + "].linear", light->getLinear());
								shader->setUniform("pointLight[" + std::to_string(i) + "].quadratic", light->getQuadratic());

								shader->setUniform("pointLight[" + std::to_string(i) + "].light.ambient", light->getAmbientColor());
								shader->setUniform("pointLight[" + std::to_string(i) + "].light.diffuse", light->getDiffuseColor());
								shader->setUniform("pointLight[" + std::to_string(i) + "].light.specular", light->getSpecularColor());
							}

							// Spot lights
							shader->setUniform("spotLightNum", static_cast<int>(command.lightingInfo.spotLights.size()));
							for (size_t i = 0; i < command.lightingInfo.spotLights.size(); ++i)
							{
								SpotLight* light = command.lightingInfo.spotLights[i];
								if (!light)
								{
									continue;
								}

								shader->setUniform("spotLight[" + std::to_string(i) + "].position", light->getPosition());
								shader->setUniform("spotLight[" + std::to_string(i) + "].direction", light->getDirection());
								shader->setUniform("spotLight[" + std::to_string(i) + "].cutOff", glm::radians(light->getCutOff()));

								shader->setUniform("spotLight[" + std::to_string(i) + "].light.ambient", light->getAmbientColor());
								shader->setUniform("spotLight[" + std::to_string(i) + "].light.diffuse", light->getDiffuseColor());
								shader->setUniform("spotLight[" + std::to_string(i) + "].light.specular", light->getSpecularColor());

								shader->setUniform("spotLight[" + std::to_string(i) + "].constant", light->getConstant());
								shader->setUniform("spotLight[" + std::to_string(i) + "].linear", light->getLinear());
								shader->setUniform("spotLight[" + std::to_string(i) + "].quadratic", light->getQuadratic());
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
	}
}

void Renderer::push(Mesh* mesh, MaterialInstance* materialInstance, Camera* camera, const glm::mat4 worldMatrix, RenderMode renderMode, float drawSize, size_t depthBufferLayer, LightingInfo directionalLights)
{
	commandsArray[depthBufferLayer].push({ mesh, materialInstance, camera, worldMatrix, renderMode, drawSize, depthBufferLayer, std::forward<LightingInfo>(directionalLights) });
}

void Renderer::push(ModelInstance* modelInstance, Camera* camera, const glm::mat4& worldMatrix, LightingInfo directionalLights)
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
		command.lightingInfo = directionalLights;
		commandsArray[command.depthBufferLayer].push(command);
	}
}
