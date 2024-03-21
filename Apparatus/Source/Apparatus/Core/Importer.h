#pragma once

class ResourceManager;

class Importer
{
public:
	Importer();
	virtual ~Importer() = default;

protected:
	ResourceManager* resourceManager;

	friend class ResourceManager;
};
