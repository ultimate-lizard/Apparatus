#pragma once

class EngineSystem
{
public:
	virtual ~EngineSystem() = default;
	virtual void init() = 0;
	virtual void uninit() = 0;
};
