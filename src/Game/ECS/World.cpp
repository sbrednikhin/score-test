#include "World.hpp"
#include "Components.hpp"
#include "MapService.hpp"
#include "SystemBase.hpp"
#include <Debug.hpp>
#include <algorithm>

namespace sw::ecs
{
	World::World()
	{
	}

	World::~World()
	{
		Deinitialize();
	}

	Entity* World::CreateEntity()
	{
		DEBUG_ASSERT(_isInitialized && "Cannot create entities before world initialization");

		uint32_t id = _nextEntityId++;
		auto entity = std::make_shared<Entity>(id);
		_entitiesStorage[id] = entity;
		return entity.get();
	}

	Entity* World::GetEntity(uint32_t id) const
	{
		auto it = _entitiesStorage.find(id);
		return it != _entitiesStorage.end() ? it->second.get() : nullptr;
	}

	Entity* World::GetEntityByExternalId(int32_t externalId) const
	{
		auto it = _externalIdToEntity.find(externalId);
		return it != _externalIdToEntity.end() ? it->second : nullptr;
	}

	Entity* World::BeginEntityInitialization()
	{
		// Создаем новую сущность для инициализации
		_currentInitializingEntity = CreateEntity();
		_currentInitializingEntity->BeginInitialization();

		return _currentInitializingEntity;
	}

	void World::EndEntityInitialization()
	{
		// Проверяем, что есть сущность в инициализации
		DEBUG_ASSERT(_currentInitializingEntity && "No entity is being initialized");

		_currentInitializingEntity->EndInitialization();

		// Если у сущности есть компонент внешнего ID, добавляем в мапу для быстрого поиска
		auto* externalId = _currentInitializingEntity->GetComponent<ExternalIdComponent>();
		if (externalId)
		{
			_externalIdToEntity[externalId->externalId] = _currentInitializingEntity;
		}

		// NOTE: PositionSystem теперь отвечает за размещение сущностей на карте
		// в фазе PreUpdate

		_currentInitializingEntity = nullptr; // Очищаем указатель
	}

	bool World::IsEntityBeingInitialized() const
	{
		return _currentInitializingEntity != nullptr;
	}


	void World::MarkEntityForDestruction(uint32_t id)
	{
		auto it = _entitiesStorage.find(id);
		if (it != _entitiesStorage.end())
		{
			it->second->MarkForDestruction();
		}
	}


	void World::RegisterSystem(std::unique_ptr<ISystem> system)
	{
		_systems.push_back(std::move(system));
	}

	void World::RegisterService(std::unique_ptr<ServiceBase> service)
	{
		_services.push_back(std::move(service));
	}

	void World::Initialize()
	{
		// На момент инициализации мир пуст, поэтому проверки не нужны
		_isInitialized = true;

		DEBUG_LOG("World initialized with " << _entitiesStorage.size() << " entities and "
				   << _systems.size() << " systems");
	}

	void World::Deinitialize()
	{
		// Деинициализируем все сущности
		for (const auto& pair : _entitiesStorage)
		{
			pair.second->Deinitialize();
		}

		_entitiesStorage.clear();
		_externalIdToEntity.clear();
		_systems.clear();
		_services.clear();
		_isInitialized = false;
	}

	void World::Update()
	{
		// PreUpdate фаза - добавление новых сущностей на карту
		for (auto& system : _systems)
		{
			system->ProcessWorldPhase(*this, UpdatePhase::PreUpdate);
		}

		// Основное обновление всех систем
		for (auto& system : _systems)
		{
			system->ProcessWorldPhase(*this, UpdatePhase::Update);
		}

		// PostUpdate фаза - очистка перед уничтожением сущностей
		for (auto& system : _systems)
		{
			system->ProcessWorldPhase(*this, UpdatePhase::PostUpdate);
		}

		// Уничтожаем все сущности, помеченные на уничтожение
		DestroyMarkedEntities();
	}

	void World::DestroyMarkedEntities()
	{
		// Находим все сущности, помеченные на уничтожение
		std::vector<uint32_t> entitiesToDestroy;

		for (const auto& pair : _entitiesStorage)
		{
			if (pair.second->IsMarkedForDestruction())
			{
				entitiesToDestroy.push_back(pair.first);
			}
		}

		// Уничтожаем найденные сущности
		for (uint32_t id : entitiesToDestroy)
		{
			auto it = _entitiesStorage.find(id);
			if (it != _entitiesStorage.end())
			{
				Entity* entity = it->second.get();

				// NOTE: PositionSystem теперь отвечает за освобождение клеток на карте
				// в фазе PostUpdate

				// Удаляем из мапы внешних ID, если есть
				auto* externalId = entity->GetComponent<ExternalIdComponent>();
				if (externalId)
				{
					_externalIdToEntity.erase(externalId->externalId);
				}

				entity->Destroy();
				_entitiesStorage.erase(it);
			}
		}
	}
}