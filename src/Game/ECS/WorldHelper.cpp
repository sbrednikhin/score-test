#include "WorldHelper.hpp"
#include "World.hpp"
#include "Components.hpp"
#include "MapService.hpp"
#include <Debug.hpp>
#include <Game/EventSystemManager.hpp>
#include <algorithm>
#include <random>

namespace sw::ecs
{
	void WorldHelper::SortEntitiesById(std::vector<Entity*>& entities)
	{
		std::sort(entities.begin(), entities.end(),
			[](Entity* a, Entity* b) {
				return a->GetId() < b->GetId();
			});
	}

	bool WorldHelper::MoveEntityTo(const World& world, Entity* entity, int32_t newX, int32_t newY)
	{
		if (!entity)
		{
			return false;
		}

		// Получаем текущую позицию
		auto* position = entity->GetComponent<PositionComponent>();
		if (!position)
		{
			return false; // Сущность не имеет позиции
		}

		// Если позиция не изменилась, ничего не делаем
		if (position->position.x == newX && position->position.y == newY)
		{
			return true;
		}

		// Получаем MapService
		auto* mapService = world.GetService<sw::ecs::MapService>();
		if (!mapService)
		{
			return false;
		}

		// Освобождаем старую клетку
		bool freed = mapService->FreeCell(position->position);
		if (!freed)
		{
			DEBUG_LOG("Warning: Failed to free cell (" << position->position.x << "," << position->position.y << ")");
			// Продолжаем, так как это может быть нормально в некоторых случаях
		}

		// Занимаем новую клетку
		bool occupied = mapService->OccupyCell({newX, newY}, entity);
		if (!occupied)
		{
			DEBUG_LOG("Warning: Failed to occupy cell (" << newX << "," << newY << ")");
			// Пытаемся вернуть старую клетку
			mapService->OccupyCell(position->position, entity);
			return false;
		}

		// Обновляем позицию сущности
		position->position = {newX, newY};

		DEBUG_LOG("Entity " << entity->GetId() << " moved to (" << newX << "," << newY << ")");

		return true;
	}

	bool WorldHelper::HasActiveEntities(const World& world)
	{
		// Получаем все сущности с BehaviourComponent
		std::vector<Entity*> entities = world.GetEntitiesWith<BehaviourComponent>();

		// Проверяем, есть ли среди них активные
		for (auto* entity : entities)
		{
			auto* behaviour = entity->GetComponent<BehaviourComponent>();
			if (behaviour && behaviour->is_active)
			{
				return true;
			}
		}

		return false;
	}

	void WorldHelper::AttackEntity(Entity* attacker, Entity* target, int32_t damage, const char* attackType)
	{
		if (!attacker || !target || damage <= 0)
		{
			return;
		}

		// Логируем атаку
		auto* attackerId = attacker->GetComponent<ExternalIdComponent>();
		auto* targetId = target->GetComponent<ExternalIdComponent>();
		auto* targetHealth = target->GetComponent<HealthComponent>();
		int32_t targetHpBefore = targetHealth ? targetHealth->health : 0;
		if (attackerId && targetId)
		{
			EventSystemManager::Get().GetEventSystem().LogUnitAttacked(attackerId->externalId, targetId->externalId, damage, targetHpBefore - damage);
		}

		// Наносим урон
		DealDamage(target, damage, attackType);
	}

	void WorldHelper::DealDamage(Entity* target, int32_t damage, const char* attackerName)
	{
		if (!target || damage <= 0)
		{
			return;
		}

		auto* targetHealth = target->GetComponent<HealthComponent>();
		if (targetHealth)
		{
			targetHealth->health -= damage;

			DEBUG_LOG(attackerName << " attacked Entity " << target->GetId()
					  << " for " << damage << " damage (health now: " << targetHealth->health << ")");

			// Если цель умерла, удаляем AliveComponent
			if (targetHealth->health <= 0)
			{
				if (target->GetComponent<AliveComponent>())
				{
					// Логируем смерть
					auto* targetId = target->GetComponent<ExternalIdComponent>();
					if (targetId)
					{
						EventSystemManager::Get().GetEventSystem().LogUnitDied(targetId->externalId);
					}

					target->RemoveComponent<AliveComponent>();
					DEBUG_LOG("Entity " << target->GetId() << " died from " << attackerName << " attack");
				}
			}
		}
	}

	Entity* WorldHelper::SelectRandomTarget(const std::vector<Entity*>& targets)
	{
		if (targets.empty())
		{
			return nullptr;
		}

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(0, targets.size() - 1);
		return targets[distrib(gen)];
	}
}