#pragma once

class AssetManager;

class Importer
{
public:
	Importer();
	virtual ~Importer() = default;

protected:
	AssetManager* assetManager;

	friend class AssetManager;
};
