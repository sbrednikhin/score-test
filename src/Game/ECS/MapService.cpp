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

    bool MapService::OccupyCell(int32_t x, int32_t y, Entity* entity)
    {
        auto* mapComponent = GetMapComponent();
        if (!mapComponent || !entity)
        {
            return false;
        }

        // Проверяем границы
        if (x < 0 || x >= mapComponent->width || y < 0 || y >= mapComponent->height)
        {
            return false;
        }

        auto key = std::make_pair(x, y);

        // Проверяем, не занята ли уже ячейка
        if (mapComponent->cells.find(key) != mapComponent->cells.end())
        {
            return false; // Ячейка уже занята
        }

        // Занимаем ячейку
        mapComponent->cells[key].entityId = entity->GetId();
        return true;
    }

    bool MapService::FreeCell(int32_t x, int32_t y)
    {
        auto* mapComponent = GetMapComponent();
        if (!mapComponent)
        {
            return false;
        }

        // Проверяем границы
        if (x < 0 || x >= mapComponent->width || y < 0 || y >= mapComponent->height)
        {
            return false;
        }

        auto key = std::make_pair(x, y);
        auto it = mapComponent->cells.find(key);

        if (it == mapComponent->cells.end())
        {
            return false; // Ячейка уже свободна
        }

        // Освобождаем ячейку - удаляем её из мапы
        mapComponent->cells.erase(it);
        return true;
    }

    bool MapService::IsCellOccupied(int32_t x, int32_t y) const
    {
        auto* mapComponent = GetMapComponent();
        if (!mapComponent)
        {
            return false;
        }

        // Проверяем границы
        if (x < 0 || x >= mapComponent->width || y < 0 || y >= mapComponent->height)
        {
            return false;
        }

        auto key = std::make_pair(x, y);
        auto it = mapComponent->cells.find(key);

        return it != mapComponent->cells.end(); // Занята если существует в мапе
    }

    Entity* MapService::GetEntityAtCell(int32_t x, int32_t y) const
    {
        auto* mapComponent = GetMapComponent();
        if (!mapComponent)
        {
            return nullptr;
        }

        // Проверяем границы
        if (x < 0 || x >= mapComponent->width || y < 0 || y >= mapComponent->height)
        {
            return nullptr;
        }

        auto key = std::make_pair(x, y);
        auto it = mapComponent->cells.find(key);

        if (it != mapComponent->cells.end())
        {
            return _world.GetEntity(it->second.entityId);
        }

        return nullptr;
    }

}
