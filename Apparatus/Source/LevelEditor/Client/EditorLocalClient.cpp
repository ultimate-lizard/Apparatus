#include "EditorLocalClient.h"

#include <Apparatus/Apparatus.h>
#include <Apparatus/Core/Logger.h>
#include <Apparatus/Core/TextureImporter.h>
#include <Apparatus/Server/LocalServer.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/LightComponent.h>
#include <Apparatus/UI/Panel/NinePatchPanel.h>

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

		if (NinePatchPanel* topPanel = uiContext.createPanel<NinePatchPanel>("Panel_TopPanel", innerWindowTexture))
		{
			topPanel->setHorizontalAlignment(Panel::Alignment::Fill);
			topPanel->setBorder(Panel::Side::Left, 6);
			topPanel->setBorder(Panel::Side::Right, 6);
			topPanel->setBorder(Panel::Side::Top, 6);
			topPanel->setBorder(Panel::Side::Bottom, 6);
		}

		if (NinePatchPanel* sidePanel = uiContext.createPanel<NinePatchPanel>("Panel_TopPanel2", thick))
		{
			sidePanel->setHorizontalAlignment(Panel::Alignment::Fill);
			sidePanel->setBorder(Panel::Side::Left, 6);
			sidePanel->setBorder(Panel::Side::Right, 6);
			sidePanel->setBorder(Panel::Side::Top, 6);
			sidePanel->setBorder(Panel::Side::Bottom, 6);

			sidePanel->setPadding(Panel::Side::Left, 16);
			sidePanel->setPadding(Panel::Side::Right, 16);
			sidePanel->setPadding(Panel::Side::Top, 16);
			sidePanel->setPadding(Panel::Side::Bottom, 16);

			sidePanel->setMargin(Panel::Side::Right, 16);
			sidePanel->setMargin(Panel::Side::Top, 48);
			sidePanel->setSize({ 444, 444 });
			sidePanel->setHorizontalAlignment(Panel::Alignment::Center);
			sidePanel->setVerticalAlignment(Panel::Alignment::Fill);

			if (NinePatchPanel* hole = uiContext.createPanel<NinePatchPanel>("Panel_TopPanel3", holeTex))
			{
				sidePanel->addChild(hole);

				hole->setBorder(Panel::Side::Left, 6);
				hole->setBorder(Panel::Side::Right, 6);
				hole->setBorder(Panel::Side::Top, 6);
				hole->setBorder(Panel::Side::Bottom, 6);
				hole->setHorizontalAlignment(Panel::Alignment::Fill);
			}
		}
	}
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
