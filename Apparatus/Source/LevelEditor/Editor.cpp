#include "Editor.h"

#include "Server/EditorLocalServer.h"
#include "Client/EditorLocalClient.h"

Editor::Editor(const std::string& gameTitle) :
	Apparatus(gameTitle)
{
}

void Editor::init()
{
	createServer<EditorLocalServer>(this);
	createClient<EditorLocalClient>(this);
}
