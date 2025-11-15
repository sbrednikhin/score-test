#include "BehaviourSystem.hpp"
#include "World.hpp"
#include "WorldHelper.hpp"
#include "../ECS/Components.hpp"
#include "Debug.hpp"

namespace sw::ecs
{
    void BehaviourSystem::ProcessWorld(World& world)
    {
        // Получаем все сущности с компонентами BehaviourComponent и HealthComponent
        std::vector<Entity*> entities = world.GetEntitiesWith<BehaviourComponent, HealthComponent>();

        for (auto* entity : entities)
        {
            // Проверяем, жива ли сущность
            if (!WorldHelper::IsAlive(*entity)) continue;

            auto* behaviour = entity->GetComponent<BehaviourComponent>();
            if (!behaviour || behaviour->behaviours.empty()) continue;

            // Выполняем поведения последовательно, пока первое не вернет true
            for (const auto& behaviourImpl : behaviour->behaviours)
            {
                DEBUG_LOG("BehaviourSystem: Entity " << entity->GetId()
                          << " trying " << behaviourImpl->GetName());

                if (behaviourImpl->Act(world, entity))
                {
                    DEBUG_LOG("BehaviourSystem: Entity " << entity->GetId()
                              << " successfully executed " << behaviourImpl->GetName());
                    break; // Выходим после успешного выполнения
                }
            }

            // Здесь можно добавить логику поведения:
            // - Для Swordsman: атака ближайших врагов
            // - Для Hunter: стрельба издалека
            // - Перемещение к цели и т.д.
        }
    }
}
