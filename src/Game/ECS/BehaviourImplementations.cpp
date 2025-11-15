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

        // Находим всех юнитов в соседних клетках (8 направлений)
        std::vector<Entity*> nearbyEntities;
        auto* mapService = world.GetService<MapService>();

        if (!mapService)
        {
            return false;
        }

        // Проверяем все 8 соседних клеток
        for (int32_t dx = -1; dx <= 1; ++dx)
        {
            for (int32_t dy = -1; dy <= 1; ++dy)
            {
                if (dx == 0 && dy == 0) continue; // Пропускаем свою клетку

                int32_t targetX = position->x + dx;
                int32_t targetY = position->y + dy;

                // Проверяем, есть ли юнит в этой клетке
                Entity* targetEntity = mapService->GetEntityAtCell(targetX, targetY);
                if (targetEntity && targetEntity != entity && WorldHelper::IsAlive(*targetEntity))
                {
                    nearbyEntities.push_back(targetEntity);
                }
            }
        }

        // Если нет целей поблизости, атака не удалась
        if (nearbyEntities.empty())
        {
            DEBUG_LOG("SwordsmanMeleeAttack: Entity " << entity->GetId() << " found no targets nearby");
            return false;
        }

        // Выбираем случайную цель
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, nearbyEntities.size() - 1);
        Entity* target = nearbyEntities[distrib(gen)];

        // Наносим урон
        auto* targetHealth = target->GetComponent<HealthComponent>();
        if (targetHealth)
        {
            int32_t damage = strength->strength;
            targetHealth->health -= damage;

            DEBUG_LOG("SwordsmanMeleeAttack: Entity " << entity->GetId()
                      << " attacked Entity " << target->GetId()
                      << " for " << damage << " damage (health now: " << targetHealth->health << ")");

            // Если цель умерла, логируем это
            if (targetHealth->health <= 0)
            {
                DEBUG_LOG("SwordsmanMeleeAttack: Entity " << target->GetId() << " died from attack");
            }
        }

        return true; // Атака удалась
    }


    bool RangeAttack::Act(World& world, Entity* entity)
    {
        // TODO: Реализовать логику дальней атаки охотника
        // Найти цель в радиусе range атаки
        // Нанести урон на расстоянии

        DEBUG_LOG("HunterRangeAttack: Entity " << entity->GetId() << " performing range attack");
        return false; // Пока возвращаем false
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
