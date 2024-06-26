#pragma once

#include <map>
#include <string>
#include <functional>

#include "InputMap.h"
#include "../Window/WindowEventHandler.h"

class UIContext;

struct KeyBinding
{
	KeyEventType type;
	std::function<void()> function;
};

struct AxisAction
{
	std::string action;
	float scale = 1.0f; // might be useful for making the axis negative
};

class InputHandler
{
public:
	InputHandler();

	void handleKey(InputKey key, KeyEventType type);
	void handleAxis(InputAxis axis, float value);
	void handleAxis(InputKey key, float value);

	void bindKeyAction(const std::string& action, KeyEventType eventType, std::function<void()> function);
	void bindAxisAction(const std::string& action, std::function<void(float)> function);

	void clearAllBindings();

	void update();

	// Adds UI Context to pass input to before dispatching the input to the delegates
	void setUIContext(UIContext* uiContext);

private:
	std::map<InputKey, std::string> keyActionMap;
	std::map<InputAxis, AxisAction> axisActionMap;
	std::map<InputKey, AxisAction> keyAsAxisActionMap;

	std::map<std::string, std::vector<KeyBinding>> keyBindingsMap;
	std::map<std::string, std::function<void(float)>> axisBindingsMap;

	std::vector<InputKey> registeredKeyAxis;

	UIContext* uiContext;
};
