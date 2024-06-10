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
#include "../UI/TopPanelMenuBuilder.h"

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
	createTopPanel();
}

void EditorLocalClient::createTopPanel()
{
	AssetManager* assetManager = Apparatus::findEngineSystem<AssetManager>();
	assert(assetManager);

	ImagePanel* topPanel = uiContext.createWidget<ImagePanel>("Panel_Top");
	topPanel->setSize({ 40, 40 });
	topPanel->setHorizontalAlignment(Widget::Alignment::Fill);
	topPanel->setTexture(assetManager->findAsset<Texture>("Texture_Panel"));
	topPanel->setMouseCaptureEnabled(true);

	Button* fileButton = uiContext.createNinePatchButton("Button_File", "Texture_Panel", "Texture_PanelInner", "Texture_PanelInnerPressed", 6, "File", 16);
	Button* editButton = uiContext.createNinePatchButton("Button_Edit", "Texture_Panel", "Texture_PanelInner", "Texture_PanelInnerPressed", 6, "Edit", 16);
	Button* windowButton = uiContext.createNinePatchButton("Button_Window", "Texture_Panel", "Texture_PanelInner", "Texture_PanelInnerPressed", 6, "Window", 16);
	Button* helpButton = uiContext.createNinePatchButton("Button_Help", "Texture_Panel", "Texture_PanelInner", "Texture_PanelInnerPressed", 6, "Help", 16);

	HorizontalPanel* topHorizontalPanel = uiContext.createWidget<HorizontalPanel>("HorizontalPanel_Top");
	topHorizontalPanel->addChild(fileButton);
	topHorizontalPanel->addChild(editButton);
	topHorizontalPanel->addChild(windowButton);
	topHorizontalPanel->addChild(helpButton);
	topHorizontalPanel->setPosition({ 4, 4 });
	topHorizontalPanel->refresh();

	TopPanelMenuBuilder::TopPanelMenuParams params;
	params.buttonBorder = 6;
	params.buttonFontSize = 16;
	params.buttonSize = { 128, 40 };
	params.idleButtonTextureName = "Texture_Panel";
	params.hoverButtonTextureName = "Texture_PanelInner";
	params.pressButtonTextureName = "Texture_PanelInnerPressed";
	params.menuBackgroundTextureName = "Texture_Panel";

	TopPanelMenuBuilder menuBuilder;
	menuBuilder.setContext(&uiContext);
	menuBuilder.setName("File_Menu");
	menuBuilder.setParams(params);
	menuBuilder.setTopPanel(topPanel);
	menuBuilder.setTopPanelMenuItem(fileButton);
	
	Button* exitButton = menuBuilder.addButton("Exit");

	Widget* fileMenu = menuBuilder.build();

	menuBuilder.setName("File_Edit");
	menuBuilder.setTopPanelMenuItem(editButton);
	Button* undoButton = menuBuilder.addButton("Undo");
	Button* redoButton = menuBuilder.addButton("Redo");
	Widget* editMenu = menuBuilder.build();

	fileButton->setCallback([editMenu, fileMenu]() {
		fileMenu->setVisibility(!fileMenu->isVisible());
		editMenu->setVisibility(false);
		});

	editButton->setCallback([editMenu, fileMenu]() {
		fileMenu->setVisibility(false);
		editMenu->setVisibility(!editMenu->isVisible());
		});

	windowButton->setCallback([editMenu, fileMenu]() {
		fileMenu->setVisibility(false);
		editMenu->setVisibility(false);
		});

	helpButton->setCallback([editMenu, fileMenu]() {
		fileMenu->setVisibility(false);
		editMenu->setVisibility(false);
		});

	exitButton->setCallback([]() {
		Apparatus::get().quit();
		});
}

ImagePanel* EditorLocalClient::createTopPanelContextMenu(const std::string& name, Button* button)
{
	AssetManager* assetManager = Apparatus::findEngineSystem<AssetManager>();
	assert(assetManager);

	ImagePanel* fileMenu = uiContext.createWidget<ImagePanel>("Panel_Context");
	fileMenu->setSize({ 128, 256 });
	fileMenu->setPosition({ 0, 40 });
	fileMenu->setTexture(assetManager->findAsset<Texture>("Texture_Panel"));
	fileMenu->setVisibility(false);

	return nullptr;
}
