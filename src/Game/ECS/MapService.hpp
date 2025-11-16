#pragma once

#include "SystemBase.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include <memory>
#include <vector>

namespace sw::ecs
{
	class MapService : public ServiceBase
	{
	public:
		explicit MapService(const World& world);
		~MapService() override;

		// ServiceBase interface
		bool IsReady() const override;

		// Регистрация сущности карты
		void SetMapEntity(Entity* mapEntity);

		// Методы работы с сеткой карты
		bool OccupyCell(int32_t x, int32_t y, Entity* entity);
		bool FreeCell(int32_t x, int32_t y);
		bool IsCellOccupied(int32_t x, int32_t y) const;
		Entity* GetEntityAtCell(int32_t x, int32_t y) const;

		// Методы поиска сущностей в радиусе
		std::vector<Entity*> GetEntitiesInRadius(int32_t centerX, int32_t centerY, int32_t radius, bool includeCenter = false) const;

		// Методы поиска сущностей в радиусе с фильтрацией по компонентам
		template<typename... Components>
		std::vector<Entity*> GetEntitiesInRadiusWith(Entity* centerEntity, int32_t radius) const
		{
			std::vector<Entity*> result;

			if (!centerEntity) return result;

			auto* position = centerEntity->GetComponent<PositionComponent>();
			if (!position) return result;

			for (int32_t dx = -radius; dx <= radius; ++dx)
			{
				for (int32_t dy = -radius; dy <= radius; ++dy)
				{
					// Проверяем, находится ли клетка в радиусе (манхэттенское расстояние)
					if (std::abs(dx) + std::abs(dy) > radius)
						continue;

					int32_t x = position->x + dx;
					int32_t y = position->y + dy;

					Entity* entity = GetEntityAtCell(x, y);
					if (entity && entity != centerEntity && (entity->GetComponent<Components>() && ...))
					{
						result.push_back(entity);
					}
				}
			}

			return result;
		}

		template<typename... Components>
		std::vector<Entity*> GetEntitiesInRangeWith(Entity* centerEntity, int32_t minRadius, int32_t maxRadius) const
		{
			std::vector<Entity*> result;

			if (!centerEntity) return result;

			auto* position = centerEntity->GetComponent<PositionComponent>();
			if (!position) return result;

			for (int32_t dx = -maxRadius; dx <= maxRadius; ++dx)
			{
				for (int32_t dy = -maxRadius; dy <= maxRadius; ++dy)
				{
					int32_t distance = std::abs(dx) + std::abs(dy);

					// Проверяем, что расстояние в заданном диапазоне
					if (distance >= minRadius && distance <= maxRadius)
					{
						int32_t x = position->x + dx;
						int32_t y = position->y + dy;

						Entity* entity = GetEntityAtCell(x, y);
						if (entity && (entity->GetComponent<Components>() && ...))
						{
							result.push_back(entity);
						}
					}
				}
			}

			return result;
		}

	private:
		// Сущность карты
		Entity* _mapEntity = nullptr;

		// Доступ к компоненту карты
		MapComponent* GetMapComponent() const;
	};
}