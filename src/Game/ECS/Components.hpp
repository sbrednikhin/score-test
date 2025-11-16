#pragma once

#include "Component.hpp"
#include <map>
#include <utility>
#include <memory>
#include <vector>

namespace sw::ecs
{
	// Forward declarations
	class Entity;
	class World;
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

	// Компонент живости (наличие = сущность жива)
	struct AliveComponent : Component
	{
		ComponentType GetType() const override { return ComponentType::Alive; }
	};

	// Интерфейс поведения
	class IBehaviour
	{
	public:
		virtual ~IBehaviour() = default;

		// Выполнить действие. Возвращает true если действие выполнено успешно
		virtual bool Act(World& world, Entity* entity) = 0;

		// Получить имя поведения для отладки
		virtual const char* GetName() const = 0;
	};

	// Поведение ближней атаки
	class MeleeAttack : public IBehaviour
	{
	public:
		bool Act(World& world, Entity* entity) override;
		const char* GetName() const override { return "MeleeAttack"; }
	};


	// Поведение дальней атаки
	class RangeAttack : public IBehaviour
	{
	public:
		bool Act(World& world, Entity* entity) override;
		const char* GetName() const override { return "RangeAttack"; }
	};

	// Поведение движения к цели
	class MoveToTarget : public IBehaviour
	{
	public:
		bool Act(World& world, Entity* entity) override;
		const char* GetName() const override { return "MoveToTarget"; }
	};

	// Компонент поведения
	struct BehaviourComponent : Component
	{
		std::vector<std::unique_ptr<IBehaviour>> behaviours;
		bool is_active = true;

		ComponentType GetType() const override { return ComponentType::Behaviour; }
	};

	// Компонент карты
	struct MapComponent : Component
	{
		int32_t width = 0;
		int32_t height = 0;

		// Sparse матрица состояний ячеек: key=(x,y), value={id сущности}
		struct CellData
		{
			uint32_t entityId = 0; // 0-свободно, иначе ID сущности
		};
		std::map<std::pair<int32_t, int32_t>, CellData> cells;

		ComponentType GetType() const override { return ComponentType::Map; }
	};

	// Компонент ловкости
	struct AgilityComponent : Component
	{
		int32_t agility = 0;

		ComponentType GetType() const override { return ComponentType::Agility; }
	};

	// Компонент дальности
	struct RangeComponent : Component
	{
		int32_t range = 0;

		ComponentType GetType() const override { return ComponentType::Range; }
	};

	// Компонент внешнего ID (ID юнита из команды)
	struct ExternalIdComponent : Component
	{
		explicit ExternalIdComponent(int32_t id) : externalId(id) {}

		const int32_t externalId;

		ComponentType GetType() const override { return ComponentType::ExternalId; }
	};

	// Компонент скорости передвижения
	struct VelocityComponent : Component
	{
		int32_t speed = 1; // Скорость передвижения (по умолчанию 1)

		ComponentType GetType() const override { return ComponentType::Velocity; }
	};
}