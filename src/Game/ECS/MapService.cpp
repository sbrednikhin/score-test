#include "MapService.hpp"
#include "World.hpp"
#include "Components.hpp"
#include <iostream>
#include <map>

namespace sw::ecs
{
	MapService::MapService(const World& world)
		: ServiceBase(world)
	{
	}

	MapService::~MapService()
	{
	}

	bool MapService::IsReady() const
	{
		return _mapEntity != nullptr;
	}

	void MapService::SetMapEntity(Entity* mapEntity)
	{
		if (mapEntity && mapEntity->GetComponent<MapComponent>())
		{
			_mapEntity = mapEntity;
		}
	}

	MapComponent* MapService::GetMapComponent() const
	{
		if (_mapEntity)
		{
			return _mapEntity->GetComponent<MapComponent>();
		}
		return nullptr;
	}

	bool MapService::OccupyCell(sw::Vec2 position, Entity* entity)
	{
		auto* mapComponent = GetMapComponent();
		if (!mapComponent || !entity)
		{
			return false;
		}

		// Проверяем границы
		if (position.x < 0 || position.x >= mapComponent->width || position.y < 0 || position.y >= mapComponent->height)
		{
			return false;
		}


		// Проверяем, не занята ли уже ячейка
		if (mapComponent->cells.find(position) != mapComponent->cells.end())
		{
			return false; // Ячейка уже занята
		}

		// Занимаем ячейку
		mapComponent->cells[position].entityId = entity->GetId();
		return true;
	}

	bool MapService::FreeCell(sw::Vec2 position)
	{
		auto* mapComponent = GetMapComponent();
		if (!mapComponent)
		{
			return false;
		}

		// Проверяем границы
		if (position.x < 0 || position.x >= mapComponent->width || position.y < 0 || position.y >= mapComponent->height)
		{
			return false;
		}

		auto it = mapComponent->cells.find(position);

		if (it == mapComponent->cells.end())
		{
			return false; // Ячейка уже свободна
		}

		// Освобождаем ячейку - удаляем её из мапы
		mapComponent->cells.erase(it);
		return true;
	}

	bool MapService::IsCellOccupied(sw::Vec2 position) const
	{
		auto* mapComponent = GetMapComponent();
		if (!mapComponent)
		{
			return false;
		}

		// Проверяем границы
		if (position.x < 0 || position.x >= mapComponent->width || position.y < 0 || position.y >= mapComponent->height)
		{
			return false;
		}

		auto it = mapComponent->cells.find(position);

		return it != mapComponent->cells.end(); // Занята если существует в мапе
	}

	Entity* MapService::GetEntityAtCell(sw::Vec2 position) const
	{
		auto* mapComponent = GetMapComponent();
		if (!mapComponent)
		{
			return nullptr;
		}

		// Проверяем границы
		if (position.x < 0 || position.x >= mapComponent->width || position.y < 0 || position.y >= mapComponent->height)
		{
			return nullptr;
		}

		auto it = mapComponent->cells.find(position);

		if (it != mapComponent->cells.end())
		{
			return _world.GetEntity(it->second.entityId);
		}

		return nullptr;
	}

	std::vector<Entity*> MapService::GetEntitiesInRadius(sw::Vec2 center, int32_t radius, bool includeCenter) const
	{
		std::vector<Entity*> result;

		for (int32_t dx = -radius; dx <= radius; ++dx)
		{
			for (int32_t dy = -radius; dy <= radius; ++dy)
			{
				// Проверяем, находится ли клетка в радиусе (манхэттенское расстояние)
				if (std::abs(dx) + std::abs(dy) > radius)
					continue;

				// Пропускаем центр, если не нужно его включать
				if (!includeCenter && dx == 0 && dy == 0)
					continue;

				int32_t x = center.x + dx;
				int32_t y = center.y + dy;

				Entity* entity = GetEntityAtCell({x, y});
				if (entity)
				{
					result.push_back(entity);
				}
			}
		}

		return result;
	}

}