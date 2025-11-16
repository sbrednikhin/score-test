#include "BehaviourSystem.hpp"
#include "World.hpp"
#include "WorldHelper.hpp"
#include "../ECS/Components.hpp"
#include "Debug.hpp"

namespace sw::ecs
{
	void BehaviourSystem::ProcessWorld(World& world)
	{
		// Получаем все сущности с компонентами BehaviourComponent и AliveComponent
		std::vector<Entity*> entities = world.GetEntitiesWith<BehaviourComponent, AliveComponent>();

		// Сортируем сущности по ID для детерминированного порядка обновления
		WorldHelper::SortEntitiesById(entities);

		for (auto* entity : entities)
		{
			auto* behaviour = entity->GetComponent<BehaviourComponent>();
			if (!behaviour || behaviour->behaviours.empty()) continue;


			// Выполняем поведения последовательно, пока первое не вернет true
			bool anyBehaviourExecuted = false;
			for (const auto& behaviourImpl : behaviour->behaviours)
			{
				DEBUG_LOG("BehaviourSystem: Entity " << entity->GetId()
						  << " trying " << behaviourImpl->GetName());

				if (behaviourImpl->Act(world, entity))
				{
					DEBUG_LOG("BehaviourSystem: Entity " << entity->GetId()
							  << " successfully executed " << behaviourImpl->GetName());
					anyBehaviourExecuted = true;
					break; // Выходим после успешного выполнения
				}
			}

			// Если ни одно поведение не сработало, деактивируем сущность
			if (!anyBehaviourExecuted)
			{
				behaviour->is_active = false;
				DEBUG_LOG("BehaviourSystem: Entity " << entity->GetId()
						  << " has no active behaviours, deactivating");
			}
		}
	}
}



