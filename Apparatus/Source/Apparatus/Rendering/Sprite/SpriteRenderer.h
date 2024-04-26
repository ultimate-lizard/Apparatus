#pragma once

#include <queue>

class Texture;
class Material;
class Sprite;
class Camera;

class SpriteRenderer
{
public:
	void setActiveCamera(Camera* camera);

	void push(Sprite* sprite);

	void render();

private:
	Camera* activeCamera;

	std::queue<Sprite*> commands;
};
