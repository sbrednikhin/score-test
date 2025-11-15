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
            auto* behaviour = entity->GetComponent<BehaviourComponent>();
            if (!behaviour || behaviour->behaviours.empty()) continue;

            // Проверяем, жива ли сущность
            if (!WorldHelper::IsAlive(*entity))
            {
                behaviour->is_active = false;
                continue;
            }

            // Выполняем поведения последовательно, пока первое не вернет true
            bool anyBehaviourExecuted = false;
            for (const auto& behaviourImpl : behaviour->behaviours)
            {
                DEBUG_LOG("BehaviourSystem: Entity " << entity->GetId()
                          << " trying " << behaviourImpl->GetName());

                if (behaviourImpl->Act(world, entity))
                {
                    DEBUG_LOG("BehaviourSystem: Entity " << entity->GetId()
                              << " successfully executed " << behaviourImpl->GetName());
                    anyBehaviourExecuted = true;
                    break; // Выходим после успешного выполнения
                }
            }

            // Если ни одно поведение не сработало, деактивируем сущность
            if (!anyBehaviourExecuted)
            {
                behaviour->is_active = false;
                DEBUG_LOG("BehaviourSystem: Entity " << entity->GetId()
                          << " has no active behaviours, deactivating");
            }
        }
    }
}
