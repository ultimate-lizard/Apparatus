#include "Asset.h"

Asset::Asset(const std::string& assetName) :
    assetName(assetName)
{
    
}

std::string Asset::getAssetName() const
{
    return assetName;
}

//AssetManager& Asset::getAssetManager()
//{
//    return assetManager;
//}
