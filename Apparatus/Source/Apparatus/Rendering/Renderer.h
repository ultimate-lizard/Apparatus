#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "Camera.h"
#include "../Core/EngineSystem.h"

using SDL_GLContext = void*; // forward declaration
struct SDL_Window;

class Mesh;
class Material;
class Model;
class Light;
class Window;
class Sprite;

enum RenderMode
{
	Points = GL_POINTS,
	Lines = GL_LINES,
	Triangles = GL_TRIANGLES,
	Quads = GL_QUADS
};

struct RenderCommand
{
	const Mesh* mesh = nullptr;
	Material* material = nullptr;
	const Camera* camera = nullptr;
	glm::mat4 worldMatrix = glm::mat4(1.0f);
	RenderMode renderMode = RenderMode::Triangles;
	float drawSize = 1.0f;
	size_t depthBufferLayer = 0;
};

class Renderer : public EngineSystem
{
public:
	Renderer(Window& window);
	~Renderer() = default;

	Renderer() = default;
	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;

	virtual void init() override;
	virtual void uninit() override {}

	void render();

	void push(Mesh* mesh, Material* material, Camera* camera, const glm::mat4 worldMatrix, RenderMode renderMode = RenderMode::Triangles, float drawSize = 1.0f, size_t depthBufferLayer = 0);
	void push(Model* model, Camera* camera, const glm::mat4& worldMatrix, size_t depthBufferLayer = 0);

	void setActiveCamera(Camera* camera);
	Camera* getActiveCamera();

	static size_t getMaxDepthBufferLayer();

	void addLight(Light* light);
	void removeLight(Light* light);

private:
	Window& window;
	SDL_GLContext context;

	std::vector<std::vector<RenderCommand>> commandsContainer;

	unsigned int uboPointLight;
	unsigned int lightsIndex;

	Camera* activeCamera;

	std::vector<Light*> lights;

	Camera spriteCamera;
};
