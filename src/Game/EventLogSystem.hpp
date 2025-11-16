#pragma once

#include "IExternalEventSystem.hpp"
#include <IO/System/EventLog.hpp>
#include <memory>

namespace sw
{
	class GameLogic;

	// Реализация системы событий через EventLog
	class EventLogSystem : public IExternalEventSystem
	{
	public:
	explicit EventLogSystem(const GameLogic& gameLogic);
	~EventLogSystem() override = default;

	const GameLogic& GetGameLogic() const override;

		void LogMapCreated(uint32_t width, uint32_t height) override;
		void LogUnitSpawned(uint32_t unitId, const char* unitType, sw::Vec2 position) override;
		void LogMarchStarted(uint32_t unitId, sw::Vec2 from, sw::Vec2 to) override;
		void LogMarchEnded(uint32_t unitId, sw::Vec2 position) override;
		void LogUnitMoved(uint32_t unitId, sw::Vec2 position) override;
		void LogUnitAttacked(uint32_t attackerId, uint32_t targetId, uint32_t damage, uint32_t targetHp) override;
		void LogUnitDied(uint32_t unitId) override;

	private:
		const GameLogic& _gameLogic;
		std::unique_ptr<EventLog> _eventLog;
	};
}