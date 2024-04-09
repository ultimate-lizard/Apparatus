#pragma once

#include <Apparatus/Apparatus.h>

class GizmoComponent;

class Editor : public Apparatus
{
public:
	Editor(const std::string& gameTitle);

protected:
	virtual void init() override;
	virtual void _createEntityTemplates() override;
};
