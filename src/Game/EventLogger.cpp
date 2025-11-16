#include "EventLogger.hpp"

namespace sw
{
	IExternalEventSystem* EventLogger::_eventSystem = nullptr;

	void EventLogger::SetEventSystem(IExternalEventSystem* eventSystem)
	{
		_eventSystem = eventSystem;
	}

	void EventLogger::LogMapCreated(uint32_t width, uint32_t height)
	{
		if (_eventSystem)
		{
			_eventSystem->LogMapCreated(width, height);
		}
	}

	void EventLogger::LogUnitSpawned(uint32_t unitId, const char* unitType, uint32_t x, uint32_t y)
	{
		if (_eventSystem)
		{
			_eventSystem->LogUnitSpawned(unitId, unitType, x, y);
		}
	}

	void EventLogger::LogMarchStarted(uint32_t unitId, uint32_t fromX, uint32_t fromY, uint32_t toX, uint32_t toY)
	{
		if (_eventSystem)
		{
			_eventSystem->LogMarchStarted(unitId, fromX, fromY, toX, toY);
		}
	}

	void EventLogger::LogMarchEnded(uint32_t unitId, uint32_t x, uint32_t y)
	{
		if (_eventSystem)
		{
			_eventSystem->LogMarchEnded(unitId, x, y);
		}
	}

	void EventLogger::LogUnitMoved(uint32_t unitId, uint32_t x, uint32_t y)
	{
		if (_eventSystem)
		{
			_eventSystem->LogUnitMoved(unitId, x, y);
		}
	}

	void EventLogger::LogUnitAttacked(uint32_t attackerId, uint32_t targetId)
	{
		if (_eventSystem)
		{
			_eventSystem->LogUnitAttacked(attackerId, targetId);
		}
	}

	void EventLogger::LogUnitDied(uint32_t unitId)
	{
		if (_eventSystem)
		{
			_eventSystem->LogUnitDied(unitId);
		}
	}
}