#pragma once

#include <vector>
#include <queue>

#include <glm/glm.hpp>
#include <glad/glad.h>

using SDL_GLContext = void*; // forward declaration
struct SDL_Window;

class Mesh;
class MaterialInstance;
class Camera;
class ModelInstance;
class DirectionalLight;
class PointLight;
class SpotLight;

enum RenderMode
{
	Points = GL_POINTS,
	Lines = GL_LINES,
	Triangles = GL_TRIANGLES,
	Quads = GL_QUADS
};

struct LightingInfo
{
	std::vector<DirectionalLight*> directionalLights;
	std::vector<PointLight*> pointLights;
	std::vector<SpotLight*> spotLights;
};

struct RenderCommand
{
	const Mesh* mesh = nullptr;
	MaterialInstance* materialInstance = nullptr;
	const Camera* camera = nullptr;
	glm::mat4 worldMatrix = glm::mat4(1.0f);
	RenderMode renderMode = RenderMode::Triangles;
	float drawSize = 1.0f;
	size_t depthBufferLayer = 0;
	LightingInfo lightingInfo;
};

class Renderer
{
public:
	Renderer(SDL_Window* window);
	~Renderer() = default;

	Renderer() = default;
	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;

	void init();

	void render();

	void push(Mesh* mesh, MaterialInstance* materialInstance, Camera* camera, const glm::mat4 worldMatrix, RenderMode renderMode = RenderMode::Triangles, float drawSize = 1.0f, size_t depthBufferLayer = 0, LightingInfo directionalLights = LightingInfo());
	void push(ModelInstance* modelInstance, Camera* camera, const glm::mat4& worldMatrix, LightingInfo directionalLights = LightingInfo());

	static size_t getMaxDepthBufferLayer()
	{
		return 7;
	}

private:
	SDL_Window* window;
	SDL_GLContext context;

	std::vector<std::queue<RenderCommand>> commandsArray;
};
