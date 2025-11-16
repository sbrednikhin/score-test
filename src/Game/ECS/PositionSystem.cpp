#include "PositionSystem.hpp"
#include "World.hpp"
#include "MapService.hpp"
#include "Components.hpp"
#include <Debug.hpp>

namespace sw::ecs
{
	void PositionSystem::ProcessWorldPhase(World& world, UpdatePhase phase)
	{
		auto* mapService = world.GetService<MapService>();
		if (!mapService || !mapService->IsReady())
			return;

		switch (phase)
		{
		case UpdatePhase::PreUpdate:
			// Добавляем новые сущности на карту
			DEBUG_LOG("PositionSystem: [PreUpdate] Processing new entities");
			ProcessNewEntities(world, mapService);
			break;

		case UpdatePhase::Update:
			// Основная фаза - ничего не делаем (позиции обрабатываются другими системами)
			break;

		case UpdatePhase::PostUpdate:
			// Удаляем сущности перед уничтожением
			DEBUG_LOG("PositionSystem: [PostUpdate] Processing entities for destruction");
			ProcessEntitiesForDestruction(world, mapService);
			break;
		}
	}

	void PositionSystem::ProcessNewEntities(World& world, MapService* mapService)
	{
		// Находим сущности в состоянии Valid (только что инициализированные)
		// которые имеют PositionComponent, но еще не на карте
		std::vector<Entity*> entitiesWithPosition = world.GetEntitiesWith<PositionComponent>();

		for (auto* entity : entitiesWithPosition)
		{
			if (entity->GetState() != EntityState::Valid)
				continue;

			auto* position = entity->GetComponent<PositionComponent>();
			if (!position)
				continue;

			// Проверяем, не занята ли уже клетка
			if (!mapService->IsCellOccupied(position->x, position->y))
			{
				bool occupied = mapService->OccupyCell(position->x, position->y, entity);
				if (occupied)
				{
					DEBUG_LOG("PositionSystem: [PreUpdate] Added entity " << entity->GetId()
							  << " to map at (" << position->x << "," << position->y << ")");
				}
				else
				{
					DEBUG_LOG("Warning: PositionSystem [PreUpdate] failed to occupy cell ("
							  << position->x << "," << position->y << ") for entity " << entity->GetId());
				}
			}
			else
			{
				// Клетка занята - это может быть нормально, если сущность уже была добавлена
				// или если несколько сущностей пытаются занять одну клетку
				Entity* occupyingEntity = mapService->GetEntityAtCell(position->x, position->y);
				if (occupyingEntity != entity)
				{
					DEBUG_LOG("Warning: PositionSystem [PreUpdate] Cell (" << position->x << "," << position->y
							  << ") already occupied by entity " << occupyingEntity->GetId()
							  << ", cannot place entity " << entity->GetId());
				}
			}
		}
	}

	void PositionSystem::ProcessEntitiesForDestruction(World& world, MapService* mapService)
	{
		// Находим сущности, помеченные на уничтожение
		std::vector<Entity*> entitiesWithPosition = world.GetEntitiesWith<PositionComponent>();

		for (auto* entity : entitiesWithPosition)
		{
			if (entity->GetState() != EntityState::MarkedForDestruction)
				continue;

			auto* position = entity->GetComponent<PositionComponent>();
			if (!position)
				continue;

			// Освобождаем клетку
			bool freed = mapService->FreeCell(position->x, position->y);
			if (freed)
			{
				DEBUG_LOG("PositionSystem: [PostUpdate] Removed entity " << entity->GetId()
						  << " from map at (" << position->x << "," << position->y << ")");
			}
			else
			{
				DEBUG_LOG("Warning: PositionSystem [PostUpdate] failed to free cell ("
						  << position->x << "," << position->y << ") for entity " << entity->GetId());
			}
		}
	}
}
