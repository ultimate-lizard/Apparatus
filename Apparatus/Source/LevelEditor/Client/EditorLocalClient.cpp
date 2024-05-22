#include "EditorLocalClient.h"

#include <Apparatus/Apparatus.h>
#include <Apparatus/Core/Logger.h>
#include <Apparatus/Core/TextureImporter.h>
#include <Apparatus/Server/LocalServer.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/LightComponent.h>
#include <Apparatus/Rendering/Sprite/Sprite.h>
#include <Apparatus/UI/Widget/HorizontalPanel.h>
#include <Apparatus/UI/Widget/TextPanel.h>

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

	if (TextureImporter* importer = Apparatus::getAssetManager().getImporter<TextureImporter>())
	{
		Texture* innerWindowTexture = Apparatus::getAssetManager().createAsset<Texture>(importer->import("Texture_WindowInner", "../Textures/WindowInner.bmp"));	
		Texture* thick = Apparatus::getAssetManager().createAsset<Texture>(importer->import("Texture_WindowThick", "../Textures/WindowThick.bmp"));
		Texture* holeTex = Apparatus::getAssetManager().createAsset<Texture>(importer->import("Texture_HoleInner", "../Textures/HoleInner.bmp"));

		auto testPanel = uiContext.createWidget<NinePatchPanel>("Panel_NinePatchPanel");
		testPanel->setTexture(thick);
		testPanel->setBorder(Widget::Side::Left, 6);
		testPanel->setBorder(Widget::Side::Right, 6);
		testPanel->setBorder(Widget::Side::Top, 6);
		testPanel->setBorder(Widget::Side::Bottom, 6);
		testPanel->setSize({ 512, 400 });
		testPanel->setHorizontalAlignment(Widget::Alignment::Fill);
		testPanel->setVerticalAlignment(Widget::Alignment::Center);

		auto innerPanel = uiContext.createWidget<NinePatchPanel>("Panel_NinePatchPanel_1");
		innerPanel->setTexture(innerWindowTexture);
		innerPanel->setBorder(Widget::Side::Left, 6);
		innerPanel->setBorder(Widget::Side::Right, 6);
		innerPanel->setBorder(Widget::Side::Top, 6);
		innerPanel->setBorder(Widget::Side::Bottom, 6);
		innerPanel->setSize({ 256, 256 });

		auto innerPanel2 = uiContext.createWidget<NinePatchPanel>("Panel_NinePatchPanel_2");
		innerPanel2->setTexture(innerWindowTexture);
		innerPanel2->setBorder(Widget::Side::Left, 6);
		innerPanel2->setBorder(Widget::Side::Right, 6);
		innerPanel2->setBorder(Widget::Side::Top, 6);
		innerPanel2->setBorder(Widget::Side::Bottom, 6);
		innerPanel2->setSize({ 256, 256 });

		auto horizontalPanel = uiContext.createWidget<HorizontalPanel>("Panel_HorizontalPanel");
		horizontalPanel->addChild(innerPanel);
		horizontalPanel->addChild(innerPanel2);
		horizontalPanel->setHorizontalAlignment(Widget::Alignment::Center);
		horizontalPanel->setVerticalAlignment(Widget::Alignment::Center);

		TextPanel* textPanel = uiContext.createWidget<TextPanel>("Panel_TextPanel");
		textPanel->setText("Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.");
		textPanel->setFontSize(14.0f);
		textPanel->setColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		textPanel->setHorizontalAlignment(Widget::Alignment::Fill);
		textPanel->setMargin(Widget::Side::Left, 8);
		textPanel->setMargin(Widget::Side::Right, 8);
		textPanel->setMargin(Widget::Side::Top, 8);
		textPanel->setMargin(Widget::Side::Bottom, 8);
		textPanel->setDepth(0.9f);
		textPanel->setColor({ 0.0f, 0.0f, 1.0f, 1.0 });

		innerPanel2->addChild(textPanel);
		innerPanel2->setDepth(0.1f);

		testPanel->addChild(horizontalPanel);

		uiContext.refreshWidgetTree();
	}
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
