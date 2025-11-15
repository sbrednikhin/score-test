#include "EventLogSystem.hpp"
#include "../GameLogic.hpp"
#include <IO/Events/MapCreated.hpp>
#include <IO/Events/UnitSpawned.hpp>
#include <IO/Events/MarchStarted.hpp>
#include <IO/Events/MarchEnded.hpp>
#include <IO/Events/UnitMoved.hpp>
#include <IO/Events/UnitAttacked.hpp>
#include <IO/Events/UnitDied.hpp>

namespace sw
{
    EventLogSystem::EventLogSystem(const GameLogic& gameLogic)
        : _gameLogic(gameLogic)
        , _eventLog(std::make_unique<EventLog>())
    {
    }

    const GameLogic& EventLogSystem::GetGameLogic() const
    {
        return _gameLogic;
    }

    void EventLogSystem::LogMapCreated(uint32_t width, uint32_t height)
    {
        if (_eventLog)
        {
            _eventLog->log(_gameLogic.GetCurrentTick(), io::MapCreated{ width, height });
        }
    }

    void EventLogSystem::LogUnitSpawned(uint32_t unitId, const char* unitType, uint32_t x, uint32_t y)
    {
        if (_eventLog)
        {
            _eventLog->log(_gameLogic.GetCurrentTick(), io::UnitSpawned{ unitId, unitType, x, y });
        }
    }

    void EventLogSystem::LogMarchStarted(uint32_t unitId, uint32_t fromX, uint32_t fromY, uint32_t toX, uint32_t toY)
    {
        if (_eventLog)
        {
            _eventLog->log(_gameLogic.GetCurrentTick(), io::MarchStarted{ unitId, fromX, fromY, toX, toY });
        }
    }

    void EventLogSystem::LogMarchEnded(uint32_t unitId, uint32_t x, uint32_t y)
    {
        if (_eventLog)
        {
            _eventLog->log(_gameLogic.GetCurrentTick(), io::MarchEnded{ unitId, x, y });
        }
    }

    void EventLogSystem::LogUnitMoved(uint32_t unitId, uint32_t x, uint32_t y)
    {
        if (_eventLog)
        {
            _eventLog->log(_gameLogic.GetCurrentTick(), io::UnitMoved{ unitId, x, y });
        }
    }

    void EventLogSystem::LogUnitAttacked(uint32_t attackerId, uint32_t targetId)
    {
        if (_eventLog)
        {
            _eventLog->log(_gameLogic.GetCurrentTick(), io::UnitAttacked{ attackerId, targetId });
        }
    }

    void EventLogSystem::LogUnitDied(uint32_t unitId)
    {
        if (_eventLog)
        {
            _eventLog->log(_gameLogic.GetCurrentTick(), io::UnitDied{ unitId });
        }
    }
}
