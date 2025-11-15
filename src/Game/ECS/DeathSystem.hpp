#pragma once

#include "System.hpp"

namespace sw::ecs
{
    class DeathSystem : public ISystem
    {
    public:
        DeathSystem() = default;
        ~DeathSystem() override = default;

        void ProcessWorld(World& world) override;
    };
}

