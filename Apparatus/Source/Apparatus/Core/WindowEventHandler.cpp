#include "WindowEventHandler.h"

#include "../Apparatus.h"
#include "../Client/InputHandler.h"

WindowEventHandler::WindowEventHandler(Apparatus* apparatus) :
	apparatus(apparatus),
	event()
{
	
}

void WindowEventHandler::handleEvents()
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			if (apparatus)
			{
				apparatus->quit();
			}
			break;

		case SDL_KEYDOWN:
			handleKeyEvent(event.key.keysym.scancode, KeyEventType::Press);
			break;

		case SDL_KEYUP:
			handleKeyEvent(event.key.keysym.scancode, KeyEventType::Release);
			break;

		case SDL_MOUSEBUTTONDOWN:
			handleMouseButtonEvent(event.button.button, KeyEventType::Press);
			break;

		case SDL_MOUSEBUTTONUP:
			handleMouseButtonEvent(event.button.button, KeyEventType::Release);
			break;

		case SDL_MOUSEWHEEL:
			handleMouseButtonEvent(event.button.button, KeyEventType::Press);
			break;

		case SDL_MOUSEMOTION:
			handleMouseMotionEvent(event.motion);
			break;

		case SDL_WINDOWEVENT:
			if (apparatus)
			{
				apparatus->onWindowResized();
			}
			break;
		}
	}

	const unsigned char* keys = SDL_GetKeyboardState(nullptr);
}

void WindowEventHandler::addInputHandler(InputHandler* inputHandler)
{
	inputHandlers.push_back(inputHandler);
}

void WindowEventHandler::handleKeyEvent(SDL_Scancode scancode, KeyEventType eventType)
{
	for (InputHandler* handler : inputHandlers)
	{
		if (!handler)
		{
			continue;
		}

		handler->handleKey(static_cast<InputKey>(scancode), eventType);
		handler->handleAxis(static_cast<InputKey>(scancode), eventType == KeyEventType::Press ? 1.0f : 0.0f);
	}
}

void WindowEventHandler::handleMouseButtonEvent(unsigned char button, KeyEventType eventType)
{
	InputKey keyButton = InputKey::MouseLeftButton;
	switch (button)
	{
	case 1:
		keyButton = InputKey::MouseLeftButton;
		break;
	case 2:
		keyButton = InputKey::MouseMiddleButton;
		break;
	case 3:
		keyButton = InputKey::MouseRightButton;
		break;
	}

	for (InputHandler* handler : inputHandlers)
	{
		if (!handler)
		{
			continue;
		}

		handler->handleKey(keyButton, eventType);
	}
}

void WindowEventHandler::handleMouseMotionEvent(SDL_MouseMotionEvent mouseEvent)
{
	for (InputHandler* handler : inputHandlers)
	{
		if (!handler)
		{
			continue;
		}

		if (mouseEvent.xrel)
		{
			handler->handleAxis(InputAxis::MouseX, static_cast<float>(mouseEvent.xrel));
		}

		if (mouseEvent.yrel)
		{
			handler->handleAxis(InputAxis::MouseY, static_cast<float>(mouseEvent.yrel));
		}
	}
}
