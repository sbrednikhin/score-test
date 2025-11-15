#pragma once

#include "ManagerBase.hpp"
#include "ECS/World.hpp"

namespace sw
{
    class WorldManager : public ManagerBase
    {
    public:
        WorldManager();
        ~WorldManager();

        void Initialize() override;
        void Deinitialize() override;
        void Update() override;

        // Доступ к миру
        ecs::World& GetWorld() { return *_world; }
        const ecs::World& GetWorld() const { return *_world; }

    private:
        std::unique_ptr<ecs::World> _world;
    };
}
