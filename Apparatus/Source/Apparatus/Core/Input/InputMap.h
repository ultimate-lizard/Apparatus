#pragma once

#include <SDL2/SDL_scancode.h>

enum InputKey
{
	A = SDL_SCANCODE_A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	Enter = SDL_SCANCODE_RETURN,
	Escape,
	Backspace,
	Tab,
	Space,
	Ctrl = SDL_SCANCODE_LCTRL,
	Shift,
	F1 = SDL_SCANCODE_F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	MouseLeftButton = 1000,
	MouseMiddleButton,
	MouseRightButton
};

enum InputAxis
{
	MouseX,
	MouseY
};
