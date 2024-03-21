#pragma once

#include <string>

class ResourceManager;

class Resource
{
public:
	Resource(const std::string& resourceName);
	virtual ~Resource() = default;

	Resource() = delete;
	Resource(const Resource&) = delete;
	Resource(Resource&&) = delete;
	void operator=(const Resource&) = delete;

	virtual void init() = 0;

	std::string getResourceName() const;

protected:
	std::string resourceName;

	ResourceManager* resourceManager;

	friend class ResourceManager;
};
