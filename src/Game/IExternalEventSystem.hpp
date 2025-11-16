#pragma once

#include <memory>
#include <cstdint>

namespace sw
{
    class GameLogic;

    // Базовый интерфейс для внешних систем событий
    class IExternalEventSystem
    {
    public:
        virtual ~IExternalEventSystem() = default;

        // Ссылка на игровую логику для получения номера тика (доступ через GetGameLogic)
        virtual const GameLogic& GetGameLogic() const = 0;

        // Универсальный интерфейс для логирования событий
        virtual void LogMapCreated(uint32_t width, uint32_t height) = 0;
        virtual void LogUnitSpawned(uint32_t unitId, const char* unitType, uint32_t x, uint32_t y) = 0;
        virtual void LogMarchStarted(uint32_t unitId, uint32_t fromX, uint32_t fromY, uint32_t toX, uint32_t toY) = 0;
        virtual void LogMarchEnded(uint32_t unitId, uint32_t x, uint32_t y) = 0;
        virtual void LogUnitMoved(uint32_t unitId, uint32_t x, uint32_t y) = 0;
        virtual void LogUnitAttacked(uint32_t attackerId, uint32_t targetId) = 0;
        virtual void LogUnitDied(uint32_t unitId) = 0;
    };
}
