#pragma once

#include "Entity.hpp"
#include "SystemBase.hpp"
#include <memory>
#include <vector>
#include <unordered_map>
#include <utility>
#include <type_traits>

namespace sw::ecs
{
    class World
    {
    public:
        World();
        ~World();

        // Управление сущностями
        Entity* GetEntity(uint32_t id) const;
        Entity* GetEntityByExternalId(int32_t externalId) const;
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

        // Регистрация сервисов
        void RegisterService(std::unique_ptr<ServiceBase> service);

        // Доступ к сервисам
        template<typename T>
        T* GetService() const
        {
            static_assert(std::is_base_of_v<ServiceBase, T>, "T must inherit from ServiceBase");
            for (const auto& service : _services)
            {
                if (auto* casted = dynamic_cast<T*>(service.get()))
                {
                    return casted;
                }
            }
            return nullptr;
        }

        // Управление жизненным циклом
        void Initialize();
        void Deinitialize();
        void Update();

    private:
        uint32_t _nextEntityId = 1;
        std::unordered_map<uint32_t, std::shared_ptr<Entity>> _entitiesStorage;

        // Мапа для быстрого поиска по external ID
        std::unordered_map<int32_t, Entity*> _externalIdToEntity;

        // Системы
        std::vector<std::unique_ptr<ISystem>> _systems;

        // Сервисы
        std::vector<std::unique_ptr<ServiceBase>> _services;

        // Текущая инициализируемая сущность
        Entity* _currentInitializingEntity = nullptr;

        // Флаг инициализации мира
        bool _isInitialized = false;

        // Вспомогательные методы
        Entity* CreateEntity();
        void DestroyMarkedEntities();
    };
}

