#include "Components.hpp"
#include "World.hpp"
#include "MapService.hpp"
#include "WorldHelper.hpp"
#include "Debug.hpp"
#include <cmath>
#include <limits>
#include <algorithm>
#include <random>
#include <vector>

namespace sw::ecs
{
    bool MeleeAttack::Act(World& world, Entity* entity)
    {
        // Получаем позицию и силу атаки текущего юнита
        auto* position = entity->GetComponent<PositionComponent>();
        auto* strength = entity->GetComponent<StrengthComponent>();

        if (!position || !strength)
        {
            return false; // Не можем атаковать без позиции или силы
        }

        // Находим всех юнитов в соседних клетках (радиус 1)
        auto* mapService = world.GetService<MapService>();
        if (!mapService)
        {
            return false;
        }

        // Получаем только живых юнитов в радиусе 1 (кроме себя)
        auto nearbyTargets = mapService->GetEntitiesInRadiusWith<AliveComponent>(entity, 1);

        // Если нет целей поблизости, атака не удалась
        if (nearbyTargets.empty())
        {
            DEBUG_LOG("MeleeAttack: Entity " << entity->GetId() << " found no targets nearby");
            return false;
        }

        // Выбираем случайную цель
        Entity* target = WorldHelper::SelectRandomTarget(nearbyTargets);
        if (!target)
        {
            return false; // Не должно случиться, но на всякий случай
        }

        // Наносим урон
        WorldHelper::DealDamage(target, strength->strength, "MeleeAttack");

        return true; // Атака удалась
    }


    bool RangeAttack::Act(World& world, Entity* entity)
    {
        DEBUG_LOG("RangeAttack: Entity " << entity->GetId() << " attempting range attack");

        // Получаем ловкость и дальность атаки
        auto* agility = entity->GetComponent<AgilityComponent>();
        auto* range = entity->GetComponent<RangeComponent>();

        if (!agility || !range)
        {
            return false; // Не можем атаковать без необходимых компонентов
        }

        auto* mapService = world.GetService<MapService>();
        if (!mapService)
        {
            return false;
        }

        // Проверяем, что в соседних клетках нет живых юнитов (радиус 1)
        auto nearbyAliveEntities = mapService->GetEntitiesInRadiusWith<AliveComponent>(entity, 1);
        if (!nearbyAliveEntities.empty())
        {
            DEBUG_LOG("RangeAttack: Entity " << entity->GetId() << " cannot shoot - enemy nearby");
            return false;
        }

        // Находим всех живых юнитов на расстоянии от 2 до Range клеток
        auto aliveDistantEntities = mapService->GetEntitiesInRangeWith<AliveComponent>(entity, 2, range->range);

        // Если нет целей в радиусе атаки, атака не удалась
        if (aliveDistantEntities.empty())
        {
            DEBUG_LOG("RangeAttack: Entity " << entity->GetId() << " found no targets in range");
            return false;
        }

        // Выбираем случайную цель
        Entity* target = WorldHelper::SelectRandomTarget(aliveDistantEntities);
        if (!target)
        {
            return false; // Не должно случиться, но на всякий случай
        }

        // Наносим урон (Agility)
        WorldHelper::DealDamage(target, agility->agility, "RangeAttack");

        return true; // Атака удалась
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
