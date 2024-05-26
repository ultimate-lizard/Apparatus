#include "Window.h"

#include <SDL2/SDL.h>

Window::Window() :
	sdlWindow(nullptr),
	wrapCursor(true)
{

}

bool Window::create(const std::string& title, const glm::ivec2& size)
{
	sdlWindow = SDL_CreateWindow(
		title.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	return sdlWindow != nullptr;
}

void Window::handleEvents()
{
	eventHandler.handleEvents();
}

void Window::swap()
{
	SDL_GL_SwapWindow(sdlWindow);
}

void Window::setCursorWrapEnabled(bool enabled)
{
	wrapCursor = enabled;
}

void Window::setCursorVisibleEnabled(bool enabled)
{
	SDL_SetRelativeMouseMode(enabled ? SDL_FALSE : SDL_TRUE);
}

void Window::setCursorPosition(const glm::ivec2& position)
{
	SDL_WarpMouseInWindow(sdlWindow, position.x, position.y);
}

bool Window::isCursorVisible() const
{
	SDL_bool result = SDL_GetRelativeMouseMode();
	return result == SDL_FALSE;
}

bool Window::isWrapCursorEnabled() const
{
	return wrapCursor;
}

glm::ivec2 Window::getWindowSize() const
{
	int width = 0;
	int height = 0;
	SDL_GetWindowSize(sdlWindow, &width, &height);

	return { width, height };
}

glm::ivec2 Window::getMouseCursorPosition() const
{
	int x = 0;
	int y = 0;

	SDL_GetMouseState(&x, &y);

	return glm::ivec2(x, y);
}

SDL_Window* Window::getSdlWindow()
{
	return sdlWindow;
}

WindowEventHandler& Window::getEventHandler()
{
	return eventHandler;
}
