#include "Entity.hpp"
#include <cassert>

namespace sw::ecs
{
    Entity::Entity(uint32_t id)
        : _id(id)
        , _state(EntityState::Uninitialized)
    {
    }

    Entity::~Entity()
    {
        // Сущность должна быть properly deinitialized через мир
        assert(_state == EntityState::Destroyed || _state == EntityState::Uninitialized);
    }

    void Entity::BeginInitialization()
    {
        assert(_state == EntityState::Uninitialized);
        _state = EntityState::Initializing;
    }

    void Entity::EndInitialization()
    {
        assert(_state == EntityState::Initializing);
        _state = EntityState::Valid;
    }

    void Entity::Deinitialize()
    {
        // Финальная очистка компонентов (может вызываться в любом состоянии)
        _components.clear();
        _componentMask.reset();
        _state = EntityState::Destroyed;
    }

    void Entity::MarkForDestruction()
    {
        assert(_state == EntityState::Valid);
        _state = EntityState::MarkedForDestruction;
    }

    void Entity::Destroy()
    {
        assert(_state == EntityState::MarkedForDestruction);

        // Финальная очистка
        Deinitialize();
        // _state уже установлен в Destroyed в Deinitialize()
    }
}

