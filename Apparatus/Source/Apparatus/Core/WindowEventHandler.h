#pragma once

#include <vector>
#include <map>
#include <SDL2/SDL_events.h>

#include "InputMap.h"

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
	WindowEventHandler(Apparatus* apparatus);

	void handleEvents();
	void addInputHandler(InputHandler* inputHandler);

private:
	void handleKeyEvent(SDL_Scancode scancode, KeyEventType eventType);
	void handleMouseButtonEvent(unsigned char button, KeyEventType eventType);
	void handleMouseMotionEvent(SDL_MouseMotionEvent mouseEvent);

	Apparatus* apparatus;
	SDL_Event event;
	std::vector<InputHandler*> inputHandlers;
};
