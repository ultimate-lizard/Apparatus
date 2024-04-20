#include "InputHandler.h"

#include "Core/Logger.h"

InputHandler::InputHandler()
{
	// TODO: This should be taken from configs
	axisActionMap.emplace(InputAxis::MouseX, AxisAction { "LookX", 1.0f });
	axisActionMap.emplace(InputAxis::MouseY, AxisAction { "LookY", 1.0f });

	keyAsAxisActionMap.emplace(InputKey::W, AxisAction{ "MoveZ", 1.0f });
	keyAsAxisActionMap.emplace(InputKey::S, AxisAction{ "MoveZ", -1.0f });

	keyAsAxisActionMap.emplace(InputKey::A, AxisAction{ "MoveX", -1.0f });
	keyAsAxisActionMap.emplace(InputKey::D, AxisAction{ "MoveX", 1.0f });

	keyAsAxisActionMap.emplace(InputKey::Q, AxisAction{ "RotateRoll", 1.0f });
	keyAsAxisActionMap.emplace(InputKey::E, AxisAction{ "RotateRoll", -1.0f });

	keyActionMap.emplace(InputKey::MouseLeftButton, "Fire");
	keyActionMap.emplace(InputKey::MouseRightButton, "AltFire");

	keyActionMap.emplace(InputKey::E, "EnableTranslationMode");
	keyActionMap.emplace(InputKey::R, "EnableRotationMode");
	keyActionMap.emplace(InputKey::T, "EnableScaleMode");

	keyActionMap.emplace(InputKey::Escape, "OpenMainMenu");

	// TODO: This is only for LevelEditor project!
	keyActionMap.emplace(InputKey::Space, "ToggleEditMode");
	keyActionMap.emplace(InputKey::G, "ToggleSnapToGrid");
	keyActionMap.emplace(InputKey::Ctrl, "EditorModifier");
	keyActionMap.emplace(InputKey::D, "Duplicate");
	keyActionMap.emplace(InputKey::Shift, "Shift");

	for (auto& pair : keyAsAxisActionMap)
	{
		registeredKeyAxis.push_back(pair.first);
	}
}

void InputHandler::handleKey(InputKey key, KeyEventType type)
{
	auto foundActionPair = keyActionMap.find(key);
	if (foundActionPair != keyActionMap.end())
	{
		auto foundBinding = keyBindingsMap.find(foundActionPair->second);
		if (foundBinding != keyBindingsMap.end())
		{
			const std::vector<KeyBinding>& bindings = foundBinding->second;
			auto bindingSearchResult = std::find_if(bindings.cbegin(), bindings.cend(), [type](const KeyBinding& b) {
				return b.type == type;
			});

			if (bindingSearchResult != bindings.cend())
			{
				bindingSearchResult->function();
			}
		}
	}
}

void InputHandler::handleAxis(InputAxis axis, float value)
{
	auto foundAxisPair = axisActionMap.find(axis);
	if (foundAxisPair != axisActionMap.end())
	{
		auto foundAxisBinding = axisBindingsMap.find(foundAxisPair->second.action);
		if (foundAxisBinding != axisBindingsMap.end())
		{
			foundAxisBinding->second(value * foundAxisPair->second.scale);
		}
	}
}

void InputHandler::handleAxis(InputKey key, float value)
{
	auto foundAxisPair = keyAsAxisActionMap.find(key);
	if (foundAxisPair != keyAsAxisActionMap.end())
	{
		auto foundAxisBinding = axisBindingsMap.find(foundAxisPair->second.action);
		if (foundAxisBinding != axisBindingsMap.end())
		{
			foundAxisBinding->second(value * foundAxisPair->second.scale);
		}
	}
}

void InputHandler::bindKeyAction(const std::string& action, KeyEventType eventType, std::function<void()> function)
{
	KeyBinding binding { eventType, function };
	keyBindingsMap[action].push_back(std::move(binding));
}

void InputHandler::bindAxisAction(const std::string& action, std::function<void(float)> function)
{
	axisBindingsMap.emplace(action, function);
}

void InputHandler::clearAllBindings()
{
	keyBindingsMap.clear();
	axisBindingsMap.clear();
}

void InputHandler::update()
{
	int keyboardLength = 0;
	const unsigned char* keyboard = SDL_GetKeyboardState(&keyboardLength);

	for (InputKey& keyAxis : registeredKeyAxis)
	{
		if (static_cast<SDL_Scancode>(keyAxis) < keyboardLength)
		{
			unsigned char key = keyboard[static_cast<SDL_Scancode>(keyAxis)];
			if (key)
			{
				handleAxis(keyAxis, 1.0f);
			}
		}
	}
}
