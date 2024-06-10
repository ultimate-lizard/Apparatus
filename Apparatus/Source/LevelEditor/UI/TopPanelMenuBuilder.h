#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

class Widget;
class Button;
class VerticalPanel;
class UIContext;

class TopPanelMenuBuilder
{
public:
	struct TopPanelMenuParams
	{
		glm::ivec2 buttonSize;
		std::string menuBackgroundTextureName;
		std::string idleButtonTextureName;
		std::string hoverButtonTextureName;
		std::string pressButtonTextureName;
		unsigned int buttonBorder;
		unsigned int buttonFontSize;
	};

	void setContext(UIContext* uiContext);
	void setName(const std::string& name);
	void setTopPanelMenuItem(Button* menuItem);
	void setTopPanel(Widget* topPanel);
	void setParams(const TopPanelMenuParams& params);

	Button* addButton(const std::string& label);

	Widget* build();

private:
	UIContext* context;

	std::string name;

	Widget* topPanel;
	Button* menuItem;

	std::vector<Button*> buttons;

	Widget* resultMenu;
	VerticalPanel* verticalPanel;

	TopPanelMenuParams params;
};