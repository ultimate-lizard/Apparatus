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
#include <Apparatus/UI/Widget/VerticalPanel.h>
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

	createUIAssets();
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

void EditorLocalClient::createUIAssets()
{
	AssetManager* assetManager = Apparatus::findEngineSystem<AssetManager>();
	if (!assetManager)
	{
		return;
	}

	TextureImporter* textureImporter = assetManager->getImporter<TextureImporter>();
	if (!textureImporter)
	{
		return;
	}

	Texture* panelTopTexture = assetManager->createAsset<Texture>("Texture_Panel", textureImporter->import("../Textures/Panel.bmp"));
	assert(panelTopTexture);
	panelTopTexture->setMinFilter(TextureFiltering::Nearest);
	panelTopTexture->setMagFilter(TextureFiltering::Nearest);

	Texture* panelTopButtonTexture = assetManager->createAsset<Texture>("Texture_PanelInner", textureImporter->import("../Textures/PanelInner.bmp"));
	assert(panelTopButtonTexture);
	panelTopButtonTexture->setMinFilter(TextureFiltering::Nearest);
	panelTopButtonTexture->setMagFilter(TextureFiltering::Nearest);

	Texture* panelTopButtonTexturePressed = assetManager->createAsset<Texture>("Texture_PanelInnerPressed", textureImporter->import("../Textures/ButtonInnerPressed.bmp"));
	assert(panelTopButtonTexturePressed);
	panelTopButtonTexturePressed->setMinFilter(TextureFiltering::Nearest);
	panelTopButtonTexturePressed->setMagFilter(TextureFiltering::Nearest);
}

void EditorLocalClient::createUI()
{
	AssetManager* assetManager = Apparatus::findEngineSystem<AssetManager>();
	if (!assetManager)
	{
		return;
	}

	ImagePanel* topPanel = uiContext.createWidget<ImagePanel>("Panel_Top");
	topPanel->setSize({ 40, 40 });
	topPanel->setHorizontalAlignment(Widget::Alignment::Fill);
	topPanel->setTexture(assetManager->findAsset<Texture>("Texture_Panel"));

	ImagePanel* fileMenu = uiContext.createWidget<ImagePanel>("Panel_Context");
	fileMenu->setSize({ 128, 256 });
	fileMenu->setPosition({ 0, 40 });
	fileMenu->setTexture(assetManager->findAsset<Texture>("Texture_Panel"));
	fileMenu->setVisibility(false);

	Button* fileButton = uiContext.createNinePatchButton("Button_File", "Texture_Panel", "Texture_PanelInner", "Texture_PanelInnerPressed", 6, "File", 16);
	Button* editButton = uiContext.createNinePatchButton("Button_Edit", "Texture_Panel", "Texture_PanelInner", "Texture_PanelInnerPressed", 6, "Edit", 16);
	Button* windowButton = uiContext.createNinePatchButton("Button_Window", "Texture_Panel", "Texture_PanelInner", "Texture_PanelInnerPressed", 6, "Window", 16);
	Button* helpButton = uiContext.createNinePatchButton("Button_Help", "Texture_Panel", "Texture_PanelInner", "Texture_PanelInnerPressed", 6, "Help", 16);

	Button* exitButton = uiContext.createNinePatchButton("Button_Exit", "Texture_Panel", "Texture_PanelInner", "Texture_PanelInnerPressed", 6, "Exit", 16);
	exitButton->setSize({ 128, 40 });
	exitButton->setSizeToContentEnabled(false);

	VerticalPanel* verticalPanel = uiContext.createWidget<VerticalPanel>("VerticalPanel_Context");
	verticalPanel->addChild(exitButton);

	fileMenu->addChild(verticalPanel);

	glm::ivec2 menuSize = fileMenu->getSize();
	fileMenu->setSize({ menuSize.x, verticalPanel->getGlobalSize().y });

	HorizontalPanel* topHorizontalPanel = uiContext.createWidget<HorizontalPanel>("HorizontalPanel_Top");
	topHorizontalPanel->addChild(fileButton);
	topHorizontalPanel->addChild(editButton);
	topHorizontalPanel->addChild(windowButton);
	topHorizontalPanel->addChild(helpButton);
	topHorizontalPanel->setPosition({ 4, 4 });

	NinePatchPanel* testPanel = uiContext.createWidget<NinePatchPanel>("NinePatchPanel_Test");
	testPanel->setPosition({ 256, 256 });
	testPanel->setTexture(assetManager->findAsset<Texture>("Texture_Panel"));
	testPanel->setSize({ 128, 64 });
	testPanel->setSizeToContentEnabled(false);

	TextPanel* testText = uiContext.createWidget<TextPanel>("TextPanel_Test");
	testText->setText("This is a very long and cool text. It should fit into the parent rectangle");
	testText->setSize({ 2, 256 });

	testPanel->addChild(testText);
}
