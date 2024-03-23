#pragma once

#include <Apparatus/Apparatus.h>

class Editor : public Apparatus
{
public:
	Editor(const std::string& gameTitle);

	virtual void init() override;
};
