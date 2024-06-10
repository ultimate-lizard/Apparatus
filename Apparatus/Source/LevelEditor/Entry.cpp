#include <Apparatus/Apparatus.h>

#include "Client/EditorLocalClient.h"
#include "Editor/Editor.h"

int main(int argc, char* argv[])
{
	Editor app("Apparatus Editor");

	return app.launch(argc, argv);
}
