#include "Asset.h"

Asset::Asset()
{
}

void Asset::setAssetName(const std::string& assetName)
{
    this->assetName = assetName;
}

const std::string& Asset::getAssetName() const
{
    return assetName;
}
