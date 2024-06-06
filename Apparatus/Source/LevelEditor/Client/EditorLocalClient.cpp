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

	Button* fileButton = uiContext.createNinePatchButton("Button_File", "Texture_Panel", "Texture_PanelInner", "Texture_PanelInnerPressed", 6, "File", 16);
	Button* editButton = uiContext.createNinePatchButton("Button_Edit", "Texture_Panel", "Texture_PanelInner", "Texture_PanelInnerPressed", 6, "Edit", 16);

	HorizontalPanel* topHorizontalPanel = uiContext.createWidget<HorizontalPanel>("HorizontalPanel_Top");
	topHorizontalPanel->addChild(fileButton);
	topHorizontalPanel->addChild(editButton);
	topHorizontalPanel->setPosition({ 4, 4 });

	NinePatchPanel* testPanel = uiContext.createWidget<NinePatchPanel>("NinePatchPanel_Test");
	testPanel->setPosition({ 256, 256 });
	testPanel->setTexture(assetManager->findAsset<Texture>("Texture_Panel"));
	testPanel->setSize({ 256, 256 });
	testPanel->setSizeToContentEnabled(true);

	TextPanel* testText = uiContext.createWidget<TextPanel>("TextPanel_Test");
	testText->setText("This is a very long and cool text. It should not fit into the parent rectangle");
	testText->setSize({ 64, 64 });
	//testText->setHorizontalAlignment(Widget::Alignment::Fill);
	//testText->setVerticalAlignment(Widget::Alignment::Fill);

	// TODO: Work out the word wrapping. It should work with the above options but it in fact doesn't
	
	testPanel->addChild(testText);
}
