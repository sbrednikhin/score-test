#pragma once

#include "Entity.hpp"
#include "System.hpp"
#include <memory>
#include <vector>
#include <unordered_map>
#include <utility>

namespace sw::ecs
{
    class World
    {
    public:
        World();
        ~World();

        // Управление сущностями
        Entity* GetEntity(uint32_t id);
        void MarkEntityForDestruction(uint32_t id);

        // Управление инициализацией сущностей
        Entity* BeginEntityInitialization();
        void EndEntityInitialization();

        // Проверка и получение текущей инициализируемой сущности
        bool IsEntityBeingInitialized() const;
        Entity* GetCurrentInitializingEntity() const
        {
            return _currentInitializingEntity;
        }

        // Получение сущностей с нужными компонентами
        template<typename... Components>
        std::vector<Entity*> GetEntitiesWith() const
        {
            std::vector<Entity*> result;

            // Создаем маску требуемых компонентов
            uint64_t requiredMask = 0;
            (void)std::initializer_list<uint64_t>{(requiredMask |= (1ULL << static_cast<size_t>(Components{}.GetType())))...};

            for (const auto& pair : _entitiesStorage)
            {
                if (pair.second->HasComponents(requiredMask))
                {
                    result.push_back(pair.second.get());
                }
            }

            return std::move(result);  // Явное применение move семантики
        }

        // Регистрация систем
        void RegisterSystem(std::unique_ptr<ISystem> system);

        // Управление жизненным циклом
        void Initialize();
        void Deinitialize();
        void Update();

    private:
        uint32_t _nextEntityId = 1;
        std::unordered_map<uint32_t, std::shared_ptr<Entity>> _entitiesStorage;

        // Системы
        std::vector<std::unique_ptr<ISystem>> _systems;

        // Текущая инициализируемая сущность
        Entity* _currentInitializingEntity = nullptr;

        // Флаг инициализации мира
        bool _isInitialized = false;

        // Вспомогательные методы
        Entity* CreateEntity();
        void DestroyMarkedEntities();
    };
}

