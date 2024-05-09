#include "EditorLocalClient.h"

#include <Apparatus/Apparatus.h>
#include <Apparatus/Core/Logger.h>
#include <Apparatus/Core/TextureImporter.h>
#include <Apparatus/Core/FontImporter.h>
#include <Apparatus/Server/LocalServer.h>
#include <Apparatus/Components/ModelComponent.h>
#include <Apparatus/Components/TransformComponent.h>
#include <Apparatus/Components/LightComponent.h>
#include <Apparatus/Rendering/Sprite/Sprite.h>


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

		if (FontImporter* fontImporter = Apparatus::getAssetManager().getImporter<FontImporter>())
		{
			std::unique_ptr<Font> importedFont = fontImporter->import("Font_Arial", "../Fonts/Arial.ttf");
			if (Font* arial = Apparatus::getAssetManager().createAsset(std::move(importedFont)))
			{
				// TODO: Implement default font in UIContext
				// TODO: Create all Texts with the default font
				testTextBlock = std::make_unique<TextBlock>(1024 * 1024);
				testTextBlock->setFont(arial);
				testTextBlock->setFontSize(14.0f);
				testTextBlock->setMaterial(uiContext.materialsMap[typeid(TextBlock)]);
				testTextBlock->setText("Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.");
				testTextBlock->setColor({ 0.0f, 0.0f, 0.0f, 1.0f });

				auto textPanel = std::make_unique<TextPanel>();
				textPanel->setTextBlock(testTextBlock.get());
				// TODO: Bring it back to the origin of y 0
				textPanel->setHorizontalAlignment(Widget::Alignment::Fill);
				textPanel->setMargin(Widget::Side::Left, 8);
				textPanel->setMargin(Widget::Side::Right, 8);
				textPanel->setMargin(Widget::Side::Top, 8);
				textPanel->setMargin(Widget::Side::Bottom, 8);

				thickWindowSprite = std::make_unique<Sprite>(256);
				thickWindowSprite->setMaterial(uiContext.materialsMap[typeid(NinePatchPanel)]);
				thickWindowSprite->setTexture(thick);

				auto testPanel = std::make_unique<NinePatchPanel>();
				testPanel->setSprite(thickWindowSprite.get());
				testPanel->setBorder(Widget::Side::Left, 6);
				testPanel->setBorder(Widget::Side::Right, 6);
				testPanel->setBorder(Widget::Side::Top, 6);
				testPanel->setBorder(Widget::Side::Bottom, 6);
				testPanel->setSize({ 512, 400 });
				testPanel->setHorizontalAlignment(Widget::Alignment::Fill);
				testPanel->setVerticalAlignment(Widget::Alignment::Center);

				thinWindowSprite = std::make_unique<Sprite>(256);
				thinWindowSprite->setMaterial(uiContext.materialsMap[typeid(NinePatchPanel)]);
				thinWindowSprite->setTexture(innerWindowTexture);

				auto innerPanel = std::make_unique<NinePatchPanel>();
				innerPanel->setSprite(thinWindowSprite.get());
				innerPanel->setBorder(Widget::Side::Left, 6);
				innerPanel->setBorder(Widget::Side::Right, 6);
				innerPanel->setBorder(Widget::Side::Top, 6);
				innerPanel->setBorder(Widget::Side::Bottom, 6);
				innerPanel->setSize({ 256, 256 });

				anotherThickSprite = std::make_unique<Sprite>(256);
				anotherThickSprite->setMaterial(uiContext.materialsMap[typeid(NinePatchPanel)]);
				anotherThickSprite->setTexture(thick);

				auto innerPanel2 = std::make_unique<NinePatchPanel>();
				innerPanel2->setSprite(anotherThickSprite.get());
				innerPanel2->setBorder(Widget::Side::Left, 6);
				innerPanel2->setBorder(Widget::Side::Right, 6);
				innerPanel2->setBorder(Widget::Side::Top, 6);
				innerPanel2->setBorder(Widget::Side::Bottom, 6);
				innerPanel2->setSize({ 256, 256 });
				innerPanel2->addChild(textPanel.get());

				auto horizontalPanel = std::make_unique<HorizontalPanel>();
				// horizontalPanel->setSize({ 0, 0 }); // if 0, 0, size to content
				horizontalPanel->addChild(innerPanel.get());
				horizontalPanel->addChild(innerPanel2.get());
				// horizontalPanel->setMargin(Widget::Side::Left, 64);
				horizontalPanel->setHorizontalAlignment(Widget::Alignment::Left);
				horizontalPanel->setVerticalAlignment(Widget::Alignment::Right);
				
				//innerPanel->setVerticalAlignment(Widget::Alignment::Fill);
				//innerPanel->setHorizontalAlignment(Widget::Alignment::Fill);

				testPanel->addChild(horizontalPanel.get());

				testPanels.push_back(std::move(testPanel));

				testPanels.push_back(std::move(innerPanel));

				testPanels.push_back(std::move(innerPanel2));

				testPanels.push_back(std::move(horizontalPanel));

				testPanels.push_back(std::move(textPanel));

				for (auto& panel : testPanels)
				{
					if (panel)
					{
						// Find root widgets and start traversing from them
						if (!panel->getParent())
						{
							panel->refresh();
						}
					}
				}
			}
		}
	}
}

void EditorLocalClient::update(float dt)
{
	LocalClient::update(dt);

	if (spriteRenderer)
	{
		for (auto iter = testPanels.rbegin(); iter != testPanels.rend(); ++iter)
		{
			auto& panel = *iter;
			if (TextPanel* textPanel = dynamic_cast<TextPanel*>(panel.get()))
			{
				spriteRenderer->push(textPanel->getTextBlock());
			}
			else if (Panel* pan = dynamic_cast<Panel*>(panel.get()))
			{
				spriteRenderer->push(pan->getSprite());
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
