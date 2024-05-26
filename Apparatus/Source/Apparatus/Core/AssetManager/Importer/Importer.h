#pragma once

class AssetManager;

class Importer
{
public:
	Importer();
	virtual ~Importer() = default;

	virtual void init() {};
	virtual void uninit() {};

protected:
	AssetManager* assetManager;

	friend class AssetManager;
};
