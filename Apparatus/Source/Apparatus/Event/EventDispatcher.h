#pragma once

#include <map>
#include <vector>
#include <typeindex>
#include <functional>
#include <memory>

#include "Event.h"
#include "../Core/EngineSystem.h"

class FuncWrapper
{
public:
	virtual ~FuncWrapper() = default;

	virtual void execute(std::shared_ptr<Event> event) = 0;
};

template <typename T>
class FuncWrapperImpl : public FuncWrapper
{
public:
	FuncWrapperImpl(std::function<void(std::shared_ptr<T>)> func) :
		func(func)
	{
		
	}

	virtual void execute(std::shared_ptr<Event> event) override
	{
		func(std::dynamic_pointer_cast<T>(event));
	}

private:
	std::function<void(std::shared_ptr<T>)> func;
};

class EventDispatcher : public EngineSystem
{
public:
	virtual void init() override {}
	virtual void uninit() override {}

	template <typename T>
	void bind(std::function<void(std::shared_ptr<T>)> func)
	{
		bindings[typeid(T)].push_back(std::make_unique<FuncWrapperImpl<T>>(func));
	}

	template <typename EventType>
	void dispatch(std::shared_ptr<EventType> event)
	{
		std::vector<std::unique_ptr<FuncWrapper>>& vec = bindings[typeid(EventType)];
		for (std::unique_ptr<FuncWrapper>& wrap : vec)
		{
			if (wrap)
			{
				wrap->execute(event);
			}
		}
	}
	
private:
	std::map<std::type_index, std::vector<std::unique_ptr<FuncWrapper>>> bindings;
};
