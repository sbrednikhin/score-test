#pragma once

#include "IExternalEventSystem.hpp"

namespace sw
{
	// Универсальный интерфейс для логирования событий внутри проекта
	class EventLogger
	{
	public:
		// Установка глобальной системы событий
		static void SetEventSystem(IExternalEventSystem* eventSystem);

		// Методы для логирования различных событий
		static void LogMapCreated(uint32_t width, uint32_t height);
		static void LogUnitSpawned(uint32_t unitId, const char* unitType, uint32_t x, uint32_t y);
		static void LogMarchStarted(uint32_t unitId, uint32_t fromX, uint32_t fromY, uint32_t toX, uint32_t toY);
		static void LogMarchEnded(uint32_t unitId, uint32_t x, uint32_t y);
		static void LogUnitMoved(uint32_t unitId, uint32_t x, uint32_t y);
		static void LogUnitAttacked(uint32_t attackerId, uint32_t targetId);
		static void LogUnitDied(uint32_t unitId);

	private:
		static IExternalEventSystem* _eventSystem;
	};
}


