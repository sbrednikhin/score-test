#include "WorldManager.hpp"
#include <Game/ECS/DeathSystem.hpp>
#include <Game/ECS/BehaviourSystem.hpp>
#include <Game/ECS/PositionSystem.hpp>
#include <Game/ECS/MapService.hpp>

namespace sw
{
	WorldManager::WorldManager()
		: _world(std::make_unique<ecs::World>())
	{
	}

	WorldManager::~WorldManager()
	{
	}

	void WorldManager::Initialize()
	{
		// Регистрируем сервисы
		_world->RegisterService(std::make_unique<ecs::MapService>(*_world));

		// Регистрируем системы (порядок важен для фаз обновления)
		_world->RegisterSystem(std::make_unique<ecs::PositionSystem>()); // PreUpdate/PostUpdate
		_world->RegisterSystem(std::make_unique<ecs::DeathSystem>());    // Update
		_world->RegisterSystem(std::make_unique<ecs::BehaviourSystem>()); // Update

		// Инициализируем мир
		_world->Initialize();
	}

	void WorldManager::Deinitialize()
	{
		if (_world) {
			_world->Deinitialize();
		}
	}

	void WorldManager::Update()
	{
		if (_world) {
			_world->Update();
		}
	}
}