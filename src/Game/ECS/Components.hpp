#pragma once

#include "Component.hpp"
#include <map>
#include <utility>
#include <memory>

namespace sw::ecs
{
    // Компонент позиции
    struct PositionComponent : Component
    {
        int32_t x = 0;
        int32_t y = 0;

        ComponentType GetType() const override { return ComponentType::Position; }
    };

    // Компонент силы
    struct StrengthComponent : Component
    {
        int32_t strength = 0;

        ComponentType GetType() const override { return ComponentType::Strength; }
    };

    // Компонент цели движения
    struct MovementTargetComponent : Component
    {
        int32_t targetX = 0;
        int32_t targetY = 0;

        ComponentType GetType() const override { return ComponentType::MovementTarget; }
    };

    // Компонент здоровья
    struct HealthComponent : Component
    {
        int32_t health = 0;

        ComponentType GetType() const override { return ComponentType::Health; }
    };

    // Компонент поведения
    enum class BehaviourType
    {
        None,
        Swordsman,
        Hunter
    };

    struct BehaviourComponent : Component
    {
        BehaviourType type = BehaviourType::None;

        ComponentType GetType() const override { return ComponentType::Behaviour; }
    };

    // Компонент карты
    struct MapComponent : Component
    {
        int32_t width = 0;
        int32_t height = 0;

        // Sparse матрица состояний ячеек: key=(x,y), value={состояние, слабая ссылка на сущность}
        struct CellData
        {
            int32_t state = 0; // 0-пусто, 1-занято
            std::weak_ptr<Entity> entity;
        };
        std::map<std::pair<int32_t, int32_t>, CellData> cells;

        ComponentType GetType() const override { return ComponentType::Map; }
    };
}

