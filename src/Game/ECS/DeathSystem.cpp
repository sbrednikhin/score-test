#include "DeathSystem.hpp"
#include "WorldHelper.hpp"
#include "World.hpp"
#include "../ECS/Components.hpp"
#include "Debug.hpp"

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
                DEBUG_LOG("DeathSystem: Marking entity " << entity->GetId() << " for destruction");

                // Помечаем сущность на уничтожение
                world.MarkEntityForDestruction(entity->GetId());

                // Здесь можно добавить дополнительную логику обработки смерти:
                // - Отправка событий и т.д.
            }
        }
    }
}
