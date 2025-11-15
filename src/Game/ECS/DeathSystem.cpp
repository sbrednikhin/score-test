#include "DeathSystem.hpp"
#include "WorldHelper.hpp"
#include "World.hpp"
#include "../ECS/Components.hpp"
#include <iostream>

namespace sw::ecs
{
    void DeathSystem::ProcessWorld(World& world)
    {
        // Получаем все сущности с HealthComponent
        std::vector<Entity*> entities = world.GetEntitiesWith<HealthComponent>();

        for (auto* entity : entities)
        {
            // Проверяем, мертва ли сущность и не помечена ли уже на уничтожение
            if (!WorldHelper::IsAlive(*entity) && !entity->IsMarkedForDestruction())
            {
                std::cout << "DeathSystem: Marking entity " << entity->GetId() << " for destruction" << std::endl;

                // Помечаем сущность на уничтожение
                world.MarkEntityForDestruction(entity->GetId());

                // Здесь можно добавить дополнительную логику обработки смерти:
                // - Отправка событий и т.д.
            }
        }
    }
}
