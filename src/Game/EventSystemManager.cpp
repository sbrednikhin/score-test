#include "EventSystemManager.hpp"
#include <cassert>

namespace sw
{
	EventSystemManager::EventSystemManager(IExternalEventSystem* eventSystem)
		: _eventSystem(eventSystem)
	{
		assert(eventSystem && "EventSystemManager: eventSystem cannot be null");
	}

	IExternalEventSystem& EventSystemManager::GetEventSystem() const
	{
		return *_eventSystem;
	}
}
