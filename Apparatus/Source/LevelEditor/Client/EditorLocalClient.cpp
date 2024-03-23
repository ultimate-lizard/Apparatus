#include "EditorLocalClient.h"

#include <Apparatus/Core/Logger.h>
#include <Apparatus/Server/LocalServer.h>

#include "EditorController.h"

EditorLocalClient::EditorLocalClient(Apparatus* apparatus) :
	LocalClient(apparatus),
	selectedEntity(nullptr),
	inEditMode(false)
{
	assignDefaultObjectName();
}

void EditorLocalClient::assignDefaultObjectName()
{
	setObjectName("EditorLocalClient");
}

void EditorLocalClient::init()
{
	LocalClient::init();

	createController<EditorController>();

	setupGlobalEditorInput();
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
	
	if (inEditMode)
	{
		setActiveController(findController<EditorController>("EditorController"));
		if (Entity* playerEntity = localServer->findEntity("Player"))
		{
			setActiveEntity(playerEntity);
		}
	}
	else
	{
		setActiveController(getDefaultController());
	}

	if (Entity* playerEntity = localServer->findEntity("Player"))
	{
		setActiveEntity(playerEntity);
	}

	if (Controller* activeController = getActiveController())
	{
		activeController->setControlEntity(getActiveEntity());
	}

	LOG(enabled ? "Edit mode" : "Free camera mode", LogLevel::Info);
}

bool EditorLocalClient::isInEditMode() const
{
	return inEditMode == true;
}

void EditorLocalClient::setupGlobalEditorInput()
{
	getInputHandler().bindKeyAction("ToggleEditMode", KeyEventType::Press, std::bind(&EditorLocalClient::toggleEditMode, this));
}
