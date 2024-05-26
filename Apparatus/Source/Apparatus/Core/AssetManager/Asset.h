#pragma once

#include <string>

class AssetManager;

class Asset
{
public:
	Asset();
	virtual ~Asset() = default;

	Asset(const Asset&) = delete;
	Asset(Asset&&) = delete;
	void operator=(const Asset&) = delete;

	virtual void init() {};
	virtual void uninit() {};

	void setAssetName(const std::string& assetName);
	const std::string& getAssetName() const;

protected:
	std::string assetName;

	friend class AssetManager;
};
