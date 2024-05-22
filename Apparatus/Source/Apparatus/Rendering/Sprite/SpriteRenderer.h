#pragma once

#include <deque>

class Texture;
class Material;
class Drawable;
class Camera;

class SpriteRenderer
{
public:
	void setActiveCamera(Camera* camera);

	void push(Drawable* sprite);

	void render();

private:
	Camera* activeCamera;

	std::deque<Drawable*> commands;
};
