#include "Entity.hpp"
#include "Debug.hpp"

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
		DEBUG_ASSERT(_state == EntityState::Destroyed || _state == EntityState::Uninitialized);
	}

	void Entity::BeginInitialization()
	{
		DEBUG_ASSERT(_state == EntityState::Uninitialized);
		_state = EntityState::Initializing;
	}

	void Entity::EndInitialization()
	{
		DEBUG_ASSERT(_state == EntityState::Initializing);
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
		DEBUG_ASSERT(_state == EntityState::Valid);
		_state = EntityState::MarkedForDestruction;
	}

	void Entity::Destroy()
	{
		DEBUG_ASSERT(_state == EntityState::MarkedForDestruction);

		// Финальная очистка
		Deinitialize();
		// _state уже установлен в Destroyed в Deinitialize()
	}
}



