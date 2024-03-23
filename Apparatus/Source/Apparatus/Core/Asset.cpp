#include "Asset.h"

Asset::Asset(const std::string& assetName) :
    assetName(assetName),
    assetManager(nullptr)
{
    
}

std::string Asset::getAssetName() const
{
    return assetName;
}
