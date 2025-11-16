#include "DeathSystem.hpp"
#include "World.hpp"
#include "Components.hpp"
#include <Debug.hpp>

namespace sw::ecs
{
	void DeathSystem::ProcessWorldPhase(World& world, UpdatePhase phase)
	{
		// DeathSystem работает только в фазе Update
		if (phase != UpdatePhase::Update)
			return;

		// Получаем все сущности с HealthComponent, но без AliveComponent (мертвые)
		std::vector<Entity*> allWithHealth = world.GetEntitiesWith<HealthComponent>();
		for (auto* entity : allWithHealth)
		{
			if (!entity->GetComponent<AliveComponent>())
			{
				DEBUG_LOG("DeathSystem: Marking dead entity " << entity->GetId() << " for destruction");

				// Помечаем мертвую сущность на уничтожение
				world.MarkEntityForDestruction(entity->GetId());
			}
		}
	}
}