#include "BehaviourSystem.hpp"
#include "World.hpp"
#include "WorldHelper.hpp"
#include "../ECS/Components.hpp"
#include <iostream>

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
            if (!behaviour || behaviour->type == BehaviourType::None) continue;

            std::cout << "BehaviourSystem: Processing entity " << entity->GetId()
                      << " with behaviour type: "
                      << (behaviour->type == BehaviourType::Swordsman ? "Swordsman" : "Hunter")
                      << std::endl;

            // Здесь можно добавить логику поведения:
            // - Для Swordsman: атака ближайших врагов
            // - Для Hunter: стрельба издалека
            // - Перемещение к цели и т.д.
        }
    }
}
