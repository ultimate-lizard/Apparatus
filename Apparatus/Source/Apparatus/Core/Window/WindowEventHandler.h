#pragma once

#include <vector>
#include <map>
#include <functional>

#include <SDL2/SDL_events.h>

#include "../Input/InputMap.h"

class Apparatus;
class InputHandler;

enum KeyEventType
{
	Press,
	Release
};

class WindowEventHandler
{
public:
	WindowEventHandler() = default;

	void handleEvents();
	void addInputHandler(InputHandler* inputHandler);

	void _bindQuitEvent(std::function<void()> function);

private:
	void handleKeyEvent(SDL_Scancode scancode, KeyEventType eventType);
	void handleMouseButtonEvent(unsigned char button, KeyEventType eventType);
	void handleMouseMotionEvent(SDL_MouseMotionEvent mouseEvent);

	void handleWindowEvent();
	void handleQuitEvent();

	SDL_Event event;
	std::vector<InputHandler*> inputHandlers;

	std::vector<std::function<void()>> quitBindings;
};
