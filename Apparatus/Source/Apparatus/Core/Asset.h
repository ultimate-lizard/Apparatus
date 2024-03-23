#pragma once

#include <string>

class AssetManager;

class Asset
{
public:
	Asset(const std::string& assetName);
	virtual ~Asset() = default;

	Asset() = delete;
	Asset(const Asset&) = delete;
	Asset(Asset&&) = delete;
	void operator=(const Asset&) = delete;

	virtual void init() = 0;

	std::string getAssetName() const;

protected:
	std::string assetName;

	AssetManager* assetManager;

	friend class AssetManager;
};
