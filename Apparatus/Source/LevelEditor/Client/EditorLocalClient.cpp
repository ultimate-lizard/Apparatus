#include "EditorLocalClient.h"

#include <Apparatus/Apparatus.h>
#include <Apparatus/Core/Logger.h>
#include <Apparatus/Core/AssetManager/Importer/TextureImporter.h>
#include <Apparatus/Core/AssetManager/AssetManager.h>
#include <Apparatus/Server/LocalServer.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/LightComponent.h>
#include <Apparatus/Rendering/Sprite/Sprite.h>
#include <Apparatus/UI/Widget/HorizontalPanel.h>
#include <Apparatus/UI/Widget/TextPanel.h>
#include <Apparatus/UI/Widget/Button.h>

#include "EditorController.h"
#include "../Components/SelectableComponent.h"
#include "../Server/EditorLocalServer.h"

EditorLocalClient::EditorLocalClient(Renderer* renderer, SpriteRenderer* spriteRenderer) :
	LocalClient(renderer, spriteRenderer),
	inEditMode(false)
{
}

void EditorLocalClient::init()
{
	LocalClient::init();

	createController<EditorController>("EditorController", *this);

	setupGlobalEditorInput();

	createUI();
}

void EditorLocalClient::update(float dt)
{
	LocalClient::update(dt);
}

void EditorLocalClient::onGameStart()
{
	LocalClient::onGameStart();
}

void EditorLocalClient::onActiveControllerChanged()
{
	setupGlobalEditorInput();
}

void EditorLocalClient::toggleEditMode()
{
	setEditModeEnabled(!isInEditMode());
}

void EditorLocalClient::setEditModeEnabled(bool enabled)
{
	inEditMode = enabled;

	Apparatus& app = Apparatus::get();

	app.getWindow().setCursorWrapEnabled(!enabled);
	
	if (inEditMode)
	{
		setActiveController(findController<EditorController>("EditorController"));
	}
	else
	{
		setActiveController(getDefaultController());
	}

	if (Controller* activeController = getActiveController())
	{
		activeController->setControlEntity(getActiveEntity());
	}
}

bool EditorLocalClient::isInEditMode() const
{
	return inEditMode == true;
}

void EditorLocalClient::setupGlobalEditorInput()
{
	getInputHandler().bindKeyAction("ToggleEditMode", KeyEventType::Press, std::bind(&EditorLocalClient::toggleEditMode, this));
}

void EditorLocalClient::createUI()
{
	if (AssetManager* assetManager = Apparatus::findEngineSystem<AssetManager>())
	{
		if (TextureImporter* importer = assetManager->getImporter<TextureImporter>())
		{
			Texture* panelTopTexture = assetManager->createAsset<Texture>("Texture_PanelTop", importer->import("../Textures/PanelTop.bmp"));
			panelTopTexture->setMinFilter(TextureFiltering::Nearest);
			panelTopTexture->setMagFilter(TextureFiltering::Nearest);

			Texture* panelTopButtonTexture = assetManager->createAsset<Texture>("Texture_PanelTopButton", importer->import("../Textures/WindowInner.bmp"));
			panelTopButtonTexture->setMinFilter(TextureFiltering::Nearest);
			panelTopButtonTexture->setMagFilter(TextureFiltering::Nearest);

			Texture * panelTopButtonTexturePressed = assetManager->createAsset<Texture>("Texture_ButtonInnerPressed", importer->import("../Textures/ButtonInnerPressed.bmp"));
			panelTopButtonTexturePressed->setMinFilter(TextureFiltering::Nearest);
			panelTopButtonTexturePressed->setMagFilter(TextureFiltering::Nearest);

			ImagePanel* topPanel = uiContext.createWidget<ImagePanel>("Panel_Top");
			topPanel->setSize({ 32, 32 });
			topPanel->setHorizontalAlignment(Widget::Alignment::Fill);
			topPanel->setTexture(panelTopTexture);

			Button* fileButton = uiContext.createWidget<Button>("Button_File");
			
			NinePatchPanel* topPanelButtonIdle = uiContext.createWidget<NinePatchPanel>("NinePatchPanel_TopPanelButtonIdle");
			topPanelButtonIdle->setTexture(panelTopTexture);
			topPanelButtonIdle->setBorder(6);
			topPanelButtonIdle->setSize({ 40, 32 });

			NinePatchPanel* topPanelButtonHover = uiContext.createWidget<NinePatchPanel>("NinePatchPanel_TopPanelButtonHover");
			topPanelButtonHover->setTexture(panelTopButtonTexture);
			topPanelButtonHover->setBorder(6);
			topPanelButtonHover->setSize({ 40, 32 });

			NinePatchPanel* topPanelButtonPress = uiContext.createWidget<NinePatchPanel>("NinePatchPanel_TopPanelButtonPress");
			topPanelButtonPress->setTexture(panelTopButtonTexturePressed);
			topPanelButtonPress->setBorder(6);
			topPanelButtonPress->setSize({ 40, 32 });
			
			TextPanel* fileLabel = uiContext.createWidget<TextPanel>("TextPanel_FileLabel");
			fileLabel->setText("File");
			fileLabel->setMargin(Widget::Side::Left, 4);
			fileLabel->setMargin(Widget::Side::Top, 4);
			fileLabel->setFontSize(18);

			fileButton->addLabel(fileLabel);
			fileButton->addPanelForState(topPanelButtonIdle, Button::ButtonState::Idle);
			fileButton->addPanelForState(topPanelButtonHover, Button::ButtonState::Hover);
			fileButton->addPanelForState(topPanelButtonPress, Button::ButtonState::Press);
		}
	}
}
