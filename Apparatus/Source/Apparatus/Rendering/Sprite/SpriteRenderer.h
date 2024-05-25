#pragma once

#include <deque>

#include "../../Core/EngineSystem/EngineSystem.h"

class Texture;
class Material;
class Drawable;
class Camera;

class SpriteRenderer : public EngineSystem
{
public:
	virtual void init() override {}
	virtual void uninit() override {}

	void setActiveCamera(Camera* camera);

	void push(Drawable* sprite);

	void render();

private:
	Camera* activeCamera;

	std::deque<Drawable*> commands;
};
