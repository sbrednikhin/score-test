#pragma once

#include "System.hpp"
#include "Entity.hpp"
#include "Components.hpp"
#include <memory>

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

    private:
        // Сущность карты
        Entity* _mapEntity = nullptr;

        // Доступ к компоненту карты
        MapComponent* GetMapComponent() const;
    };
}
