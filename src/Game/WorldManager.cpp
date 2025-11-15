#include "WorldManager.hpp"
#include "ECS/DeathSystem.hpp"
#include "ECS/BehaviourSystem.hpp"

namespace sw
{
    WorldManager::WorldManager()
        : _world(std::make_unique<ecs::World>())
    {
    }

    WorldManager::~WorldManager()
    {
    }

    void WorldManager::Initialize()
    {
        // Регистрируем системы
        _world->RegisterSystem(std::make_unique<ecs::DeathSystem>());
        _world->RegisterSystem(std::make_unique<ecs::BehaviourSystem>());

        // Инициализируем мир
        _world->Initialize();
    }

    void WorldManager::Deinitialize()
    {
        if (_world) {
            _world->Deinitialize();
        }
    }

    void WorldManager::Update()
    {
        if (_world) {
            _world->Update();
        }
    }
}
