#pragma once

#include <string>
#include <set>

class NameProvider
{
public:
	NameProvider();
	NameProvider(const std::string& name);

	const std::string& getObjectName() const;

protected:
	void setObjectName(const std::string& name);
	virtual void assignDefaultObjectName() = 0;

private:
	std::string name;
};
