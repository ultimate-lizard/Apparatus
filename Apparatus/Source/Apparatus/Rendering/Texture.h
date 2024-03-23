#pragma once

#include <string>

#include "../Core/Asset.h"

class Texture : public Asset
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
