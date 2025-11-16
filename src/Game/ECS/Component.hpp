#pragma once

#include <cstdint>

namespace sw::ecs
{
	// Типы компонентов для идентификации и битовых масок
	enum class ComponentType : size_t
	{
		Position,
		Strength,
		MovementTarget,
		Health,
		Alive,
		Behaviour,
		Map,
		Agility,
		Range,
		ExternalId,
		Velocity,

		// Служебное значение - количество компонентов
		COUNT
	};

	// Максимальное количество компонентов
	constexpr size_t MAX_COMPONENTS = static_cast<size_t>(ComponentType::COUNT);

	struct Component
	{
		Component() = default;
		virtual ~Component() = default;

		// Разрешаем копирование для компонентов
		Component(const Component&) = default;
		Component& operator=(const Component&) = default;

		// Разрешаем перемещение
		Component(Component&&) = default;
		Component& operator=(Component&&) = default;

		// Получение типа компонента (должно быть переопределено в наследниках)
		virtual ComponentType GetType() const = 0;

		// Получение бита компонента на основе типа
		size_t GetComponentBit() const
		{
			return static_cast<size_t>(GetType());
		}

	};
}



