#pragma once

#include <string>

#include "../Core/Resource.h"

class Texture : public Resource
{
public:
	Texture(const std::string& resourceName, const std::string& texturePath);
	~Texture();

	Texture() = delete;
	Texture(Texture&&) = delete;
	Texture(const Texture&) = delete;
	void operator=(const Texture&) = delete;

	virtual void init() override;

	void bind() const;

private:
	std::string texturePath;
	unsigned int id;
};
