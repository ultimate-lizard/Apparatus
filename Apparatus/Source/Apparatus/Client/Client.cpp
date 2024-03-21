#include "Client.h"

Client::Client(Apparatus* apparatus) :
	apparatus(apparatus),
	quitting(false)
{
}

void Client::quit()
{
	quitting = true;
}

Apparatus* Client::getApparatus()
{
	return apparatus;
}
