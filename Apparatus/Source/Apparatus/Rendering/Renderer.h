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
class PointLight;
class Window;
class DirectionalLight;
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
	std::queue<DirectionalLight*> directionalLights;
	std::queue<PointLight*> pointLights;
	std::queue<SpotLight*> spotLights;
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
};

class Renderer
{
public:
	Renderer(Window& window);
	~Renderer() = default;

	Renderer() = default;
	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;

	void init();

	void render();

	void push(Mesh* mesh, MaterialInstance* materialInstance, Camera* camera, const glm::mat4 worldMatrix, RenderMode renderMode = RenderMode::Triangles, float drawSize = 1.0f, size_t depthBufferLayer = 0);
	void push(ModelInstance* modelInstance, Camera* camera, const glm::mat4& worldMatrix);

	void setActiveCamera(Camera* camera);
	Camera* getActiveCamera();

	static size_t getMaxDepthBufferLayer();

	void addPointLight(PointLight* pointLight);
	void removeLight(PointLight* pointLight);

private:
	Window& window;
	SDL_GLContext context;

	std::vector<std::vector<RenderCommand>> commandsContainer;

	unsigned int uboPointLight;
	unsigned int lightsIndex;

	Camera* activeCamera;

	std::vector<PointLight*> pointLights;
};
