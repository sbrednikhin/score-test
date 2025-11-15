#include "Components.hpp"
#include "World.hpp"
#include "MapService.hpp"
#include "WorldHelper.hpp"
#include "Debug.hpp"
#include <cmath>
#include <limits>
#include <algorithm>

namespace sw::ecs
{
    bool SwordsmanMeleeAttack::Act(World& world, Entity* entity)
    {
        // TODO: Реализовать логику ближней атаки мечника
        // Найти ближайшего врага в радиусе melee атаки
        // Нанести урон

        DEBUG_LOG("SwordsmanMeleeAttack: Entity " << entity->GetId() << " performing melee attack");
        return false; // Временно возвращаем false, чтобы дать шанс движению
    }

    bool HunterMeleeAttack::Act(World& world, Entity* entity)
    {
        // TODO: Реализовать логику ближней атаки охотника
        // Аналогично SwordsmanMeleeAttack, но возможно с другими характеристиками

        DEBUG_LOG("HunterMeleeAttack: Entity " << entity->GetId() << " performing melee attack");
        return false; // Временно возвращаем false, чтобы дать шанс движению
    }

    bool HunterRangeAttack::Act(World& world, Entity* entity)
    {
        // TODO: Реализовать логику дальней атаки охотника
        // Найти цель в радиусе range атаки
        // Нанести урон на расстоянии

        DEBUG_LOG("HunterRangeAttack: Entity " << entity->GetId() << " performing range attack");
        return false; // Временно возвращаем false, чтобы дать шанс движению
    }

    bool MoveToTarget::Act(World& world, Entity* entity)
    {
        // Получаем компоненты
        auto* position = entity->GetComponent<PositionComponent>();
        auto* movementTarget = entity->GetComponent<MovementTargetComponent>();
        auto* velocity = entity->GetComponent<VelocityComponent>();

        if (!position || !movementTarget || !velocity)
        {
            return false; // Не можем двигаться без необходимых компонентов
        }

        // Проверяем, достигли ли уже цели
        if (position->x == movementTarget->targetX && position->y == movementTarget->targetY)
        {
            // Удаляем компонент цели при достижении
            entity->RemoveComponent<MovementTargetComponent>();
            DEBUG_LOG("Entity " << entity->GetId() << " reached target and removed MovementTargetComponent");
            return false; // Уже на месте, ничего не делаем
        }

        // Получаем MapService
        auto* mapService = world.GetService<sw::ecs::MapService>();
        if (!mapService)
        {
            return false;
        }

        // Ищем лучшую соседнюю клетку для приближения к цели
        int32_t bestX = position->x;
        int32_t bestY = position->y;
        double bestDistance = std::numeric_limits<double>::max();

        // Проверяем все 8 возможных направлений движения
        for (int32_t dx = -1; dx <= 1; ++dx)
        {
            for (int32_t dy = -1; dy <= 1; ++dy)
            {
                // Пропускаем текущую позицию
                if (dx == 0 && dy == 0) continue;

                int32_t candidateX = position->x + dx;
                int32_t candidateY = position->y + dy;

                // Проверяем границы карты (простая проверка на неотрицательные координаты)
                // TODO: Добавить проверку границ карты через MapService
                if (candidateX < 0 || candidateY < 0)
                {
                    continue; // Отрицательные координаты не допускаем
                }

                // Проверяем, что клетка свободна
                if (mapService->IsCellOccupied(candidateX, candidateY))
                {
                    continue; // Клетка занята
                }

                // Вычисляем расстояние от кандидата до цели
                double distanceToTarget = std::sqrt(
                    std::pow(candidateX - movementTarget->targetX, 2) +
                    std::pow(candidateY - movementTarget->targetY, 2)
                );

                // Если это лучшее расстояние, запоминаем клетку
                if (distanceToTarget < bestDistance)
                {
                    bestDistance = distanceToTarget;
                    bestX = candidateX;
                    bestY = candidateY;
                }
            }
        }

        // Если не нашли подходящую клетку, не двигаемся
        if (bestX == position->x && bestY == position->y)
        {
            return false;
        }

        int32_t newX = bestX;
        int32_t newY = bestY;

        // Перемещаем сущность на новую позицию
        return WorldHelper::MoveEntityTo(world, entity, newX, newY);
    }
}
