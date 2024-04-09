#pragma once

#include <string>

#include <glm/glm.hpp>

#include "WindowEventHandler.h"

struct SDL_Window;

class Window
{
public:
	Window();

	bool create(const std::string& title = "Apparatus", const glm::ivec2& size = {800, 600});

	void handleEvents();
	void swap();

	void setCursorWrapEnabled(bool enabled);
	void setCursorVisibleEnabled(bool enabled);
	void setCursorPosition(const glm::ivec2& position);
	bool isCursorVisible() const;
	bool isWrapCursorEnabled() const;

	glm::ivec2 getWindowSize() const;
	glm::ivec2 getMouseCursorPosition() const;

	SDL_Window* getSdlWindow();

	WindowEventHandler& getEventHandler();

private:
	SDL_Window* sdlWindow;

	bool wrapCursor;

	WindowEventHandler eventHandler;
};
