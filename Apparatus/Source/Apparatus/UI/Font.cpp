#include "Font.h"

void Font::addCharacterSet(CharacterSet&& characterSet)
{
	// CharacterSet characterSet;

	characterSetMap[characterSet.fontSize] = std::forward<CharacterSet>(characterSet);
}

CharacterSet* Font::getCharacterSet(size_t fontSize)
{
	auto setSearchIter = characterSetMap.find(fontSize);
	if (setSearchIter != characterSetMap.cend())
	{
		return &setSearchIter->second;
	}

	return nullptr;
}

CharacterSet* Font::getCurrentCharacterSet()
{
	return getCharacterSet(fontSize);
}

Font::Font(const std::string& assetName) :
	Asset(assetName),
	fontSize(14)
{

}
