#include "NameProvider.h"

#include <map>
#include "Logger.h"

static std::map<std::string, size_t> globalNameMap;

NameProvider::NameProvider()
{
	
}

NameProvider::NameProvider(const std::string& name) :
	name(name)
{
	
}

const std::string& NameProvider::getObjectName() const
{
	return name;
}

void NameProvider::setObjectName(const std::string& name)
{
	auto iter = globalNameMap.find(name);
	if (iter == globalNameMap.end())
	{
		globalNameMap.emplace(name, 0);
		this->name = name;
	}
	else
	{
		size_t index = ++globalNameMap[name];
		this->name = name + "_" + std::to_string(index);
		// LOG("GlobalNameMap already has name '" + name + "'! Automatically added index to the name. The new name is '" + this->name + "'", LogLevel::Warning);
	}
}
