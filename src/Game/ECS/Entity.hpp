#pragma once

#include "Component.hpp"
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <bitset>
#include <cassert>

namespace sw::ecs
{
	// Состояния жизненного цикла сущности
	enum class EntityState
	{
		Uninitialized,	  // Начальное состояние
		Initializing,	   // Во время инициализации
		Valid,			 // Готова к использованию
		MarkedForDestruction, // Помечена на уничтожение
		Destroyed		  // Уничтожена
	};

	class Entity
	{
	public:
	explicit Entity(uint32_t id);
	~Entity();

	uint32_t GetId() const { return _id; }

	// Получение состояния
	EntityState GetState() const { return _state; }

	// Проверка состояния (для обратной совместимости)
	bool IsValid() const { return _state == EntityState::Valid; }
	bool IsInitializing() const { return _state == EntityState::Initializing; }
	bool IsMarkedForDestruction() const { return _state == EntityState::MarkedForDestruction; }


		// Добавление компонентов (в любое время)
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

			auto component = std::make_unique<T>(std::forward<Args>(args)...);
			auto typeIndex = std::type_index(typeid(T));

			// Устанавливаем бит в маске
			_componentMask[static_cast<size_t>(component->GetType())] = true;

			T* ptr = component.get();
			_components[typeIndex] = std::move(component);
			return *ptr;
		}

		// Удаление компонентов (в любое время)
		template<typename T>
		void RemoveComponent()
		{
			static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

			auto typeIndex = std::type_index(typeid(T));

			// Сбрасываем бит в маске
			_componentMask[static_cast<size_t>(T{}.GetType())] = false;

			// Удаляем компонент
			_components.erase(typeIndex);
		}

		// Получение компонентов
		template<typename T>
		T* GetComponent()
		{
			static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

			auto typeIndex = std::type_index(typeid(T));
			auto it = _components.find(typeIndex);
			return it != _components.end() ? static_cast<T*>(it->second.get()) : nullptr;
		}

		template<typename T>
		const T* GetComponent() const
		{
			static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

			auto typeIndex = std::type_index(typeid(T));
			auto it = _components.find(typeIndex);
			return it != _components.end() ? static_cast<const T*>(it->second.get()) : nullptr;
		}

		// Проверка наличия компонентов
		template<typename T>
		bool HasComponent() const
		{
			static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
			return _componentMask[static_cast<size_t>(T{}.GetType())];
		}

		// Проверка по битовой маске
		bool HasComponents(uint64_t mask) const
		{
			return (_componentMask.to_ullong() & mask) == mask;
		}

		uint64_t GetComponentMask() const
		{
			return _componentMask.to_ullong();
		}


	private:
		// Управление жизненным циклом (доступ только через friend World)
		void BeginInitialization();
		void EndInitialization();
		void Deinitialize();
		void MarkForDestruction();
		void Destroy();

		// Доступ для мира к приватным методам жизненного цикла
		friend class World;

		uint32_t _id;
		EntityState _state;
		std::bitset<MAX_COMPONENTS> _componentMask;
		std::unordered_map<std::type_index, std::unique_ptr<Component>> _components;
	};
}


