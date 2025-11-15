#pragma once

#include <cstdint>

namespace sw::ecs
{
    class Entity;
    class World;

    class ISystem
    {
    public:
        virtual ~ISystem() = default;

        // Основной метод обработки мира
        virtual void ProcessWorld(World& world) = 0;
    };
}

