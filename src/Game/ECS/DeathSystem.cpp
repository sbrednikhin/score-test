#include "DeathSystem.hpp"
#include "WorldHelper.hpp"
#include "World.hpp"
#include "../ECS/Components.hpp"
#include "Debug.hpp"

namespace sw::ecs
{
	void DeathSystem::ProcessWorld(World& world)
	{
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