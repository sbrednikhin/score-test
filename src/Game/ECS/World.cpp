#include "World.hpp"
#include <iostream>
#include <cassert>
#include <algorithm>

namespace sw::ecs
{
    World::World()
    {
    }

    World::~World()
    {
        Deinitialize();
    }

    Entity* World::CreateEntity()
    {
        assert(_isInitialized && "Cannot create entities before world initialization");

        uint32_t id = _nextEntityId++;
        auto entity = std::make_shared<Entity>(id);
        _entitiesStorage[id] = entity;
        return entity.get();
    }

    Entity* World::GetEntity(uint32_t id)
    {
        auto it = _entitiesStorage.find(id);
        return it != _entitiesStorage.end() ? it->second.get() : nullptr;
    }

    Entity* World::BeginEntityInitialization()
    {
        // Создаем новую сущность для инициализации
        _currentInitializingEntity = CreateEntity();
        _currentInitializingEntity->BeginInitialization();

        return _currentInitializingEntity;
    }

    void World::EndEntityInitialization()
    {
        // Проверяем, что есть сущность в инициализации
        assert(_currentInitializingEntity && "No entity is being initialized");

        _currentInitializingEntity->EndInitialization();
        _currentInitializingEntity = nullptr; // Очищаем указатель
    }

    bool World::IsEntityBeingInitialized() const
    {
        return _currentInitializingEntity != nullptr;
    }


    void World::MarkEntityForDestruction(uint32_t id)
    {
        auto it = _entitiesStorage.find(id);
        if (it != _entitiesStorage.end())
        {
            it->second->MarkForDestruction();
        }
    }


    void World::RegisterSystem(std::unique_ptr<ISystem> system)
    {
        _systems.push_back(std::move(system));
    }

    void World::Initialize()
    {
        // На момент инициализации мир пуст, поэтому проверки не нужны
        _isInitialized = true;

        std::cout << "World initialized with " << _entitiesStorage.size() << " entities and "
                  << _systems.size() << " systems" << std::endl;
    }

    void World::Deinitialize()
    {
        // Деинициализируем все сущности
        for (const auto& pair : _entitiesStorage)
        {
            pair.second->Deinitialize();
        }

        _entitiesStorage.clear();
        _systems.clear();
        _isInitialized = false;
    }

    void World::Update()
    {
        // Обновляем все системы
        for (auto& system : _systems)
        {
            system->ProcessWorld(*this);
        }

        // Уничтожаем все сущности, помеченные на уничтожение
        DestroyMarkedEntities();
    }

    void World::DestroyMarkedEntities()
    {
        // Находим все сущности, помеченные на уничтожение
        std::vector<uint32_t> entitiesToDestroy;

        for (const auto& pair : _entitiesStorage)
        {
            if (pair.second->IsMarkedForDestruction())
            {
                entitiesToDestroy.push_back(pair.first);
            }
        }

        // Уничтожаем найденные сущности
        for (uint32_t id : entitiesToDestroy)
        {
            auto it = _entitiesStorage.find(id);
            if (it != _entitiesStorage.end())
            {
                it->second->Destroy();
                _entitiesStorage.erase(it);
            }
        }
    }
}
