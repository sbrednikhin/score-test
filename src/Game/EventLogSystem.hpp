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
        void LogUnitSpawned(uint32_t unitId, const char* unitType, uint32_t x, uint32_t y) override;
        void LogMarchStarted(uint32_t unitId, uint32_t fromX, uint32_t fromY, uint32_t toX, uint32_t toY) override;
        void LogMarchEnded(uint32_t unitId, uint32_t x, uint32_t y) override;
        void LogUnitMoved(uint32_t unitId, uint32_t x, uint32_t y) override;
        void LogUnitAttacked(uint32_t attackerId, uint32_t targetId) override;
        void LogUnitDied(uint32_t unitId) override;

    private:
        const GameLogic& _gameLogic;
        std::unique_ptr<EventLog> _eventLog;
    };
}
