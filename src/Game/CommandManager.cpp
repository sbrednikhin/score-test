#include "CommandManager.hpp"
#include "WorldManager.hpp"
#include "ECS/Components.hpp"
#include "ECS/MapService.hpp"
#include "Debug.hpp"
#include <cstring>

namespace sw
{
	CommandManager::CommandManager()
	{
	}

	CommandManager::~CommandManager()
	{
	}

	void CommandManager::Initialize()
	{
		_commands.clear();
	}

	void CommandManager::Deinitialize()
	{
		_commands.clear();
	}

	void CommandManager::Update()
	{
		// Сначала проходим по всем командам и обрабатываем их
		for (const auto& command : _commands)
		{
			const char* commandName = command->GetTypeName();

			if (strcmp(commandName, sw::io::CreateMap::TypeName) == 0) {
				ProcessCreateMap(*static_cast<const sw::io::CreateMap*>(command.get()));
			}
			else if (strcmp(commandName, sw::io::SpawnSwordsman::TypeName) == 0) {
				ProcessSpawnSwordsman(*static_cast<const sw::io::SpawnSwordsman*>(command.get()));
			}
			else if (strcmp(commandName, sw::io::SpawnHunter::TypeName) == 0) {
				ProcessSpawnHunter(*static_cast<const sw::io::SpawnHunter*>(command.get()));
			}
			else if (strcmp(commandName, sw::io::March::TypeName) == 0) {
				ProcessMarch(*static_cast<const sw::io::March*>(command.get()));
			}
		}

		// После обработки всех команд очищаем очередь
		ClearCommands();
	}

	void CommandManager::AddCommand(std::shared_ptr<sw::io::ICommand> command)
	{
		if (command)
		{
			// Временное логгирование для отладки
			DEBUG_LOG(command->GetTypeName() << " command added (ID: " << command->GetTypeId() << ")");
			_commands.push_back(command);
		}
	}

	const std::vector<std::shared_ptr<sw::io::ICommand>>& CommandManager::GetCommands() const
	{
		return _commands;
	}

	void CommandManager::ClearCommands()
	{
		_commands.clear();
	}

	void CommandManager::ProcessCreateMap(const sw::io::CreateMap& command)
	{
		// Получаем доступ к WorldManager и создаем карту через ECS
		auto& worldManager = WorldManager::Get();
		auto& world = worldManager.GetWorld();

		DEBUG_LOG("Processing CreateMap: width=" << command.width << ", height=" << command.height);

		// Создаем и инициализируем сущность с компонентом карты
		auto mapEntity = world.BeginEntityInitialization();
		auto& mapComponent = mapEntity->AddComponent<sw::ecs::MapComponent>();
		mapComponent.width = command.width;
		mapComponent.height = command.height;

		// Завершаем инициализацию сущности
		world.EndEntityInitialization();

		// Регистрируем сущность карты в сервисе
		if (auto* mapService = world.GetService<ecs::MapService>())
		{
			mapService->SetMapEntity(mapEntity);
		}

		DEBUG_LOG("Created map entity with ID: " << mapEntity->GetId());
	}

	void CommandManager::ProcessSpawnSwordsman(const sw::io::SpawnSwordsman& command)
	{
		auto& worldManager = WorldManager::Get();
		auto& world = worldManager.GetWorld();

		DEBUG_LOG("Processing SpawnSwordsman: id=" << command.unitId
				   << ", pos=(" << command.x << "," << command.y << ")"
				   << ", hp=" << command.hp << ", strength=" << command.strength);

		// Создаем сущность мечника
		auto swordsmanEntity = world.BeginEntityInitialization();

		// Добавляем компоненты
		auto& position = swordsmanEntity->AddComponent<sw::ecs::PositionComponent>();
		position.x = command.x;
		position.y = command.y;

		auto& health = swordsmanEntity->AddComponent<sw::ecs::HealthComponent>();
		health.health = command.hp;

		// Добавляем компонент живости (сущность жива)
		swordsmanEntity->AddComponent<sw::ecs::AliveComponent>();

		auto& strength = swordsmanEntity->AddComponent<sw::ecs::StrengthComponent>();
		strength.strength = command.strength;

		swordsmanEntity->AddComponent<sw::ecs::ExternalIdComponent>(command.unitId);


		auto& velocity = swordsmanEntity->AddComponent<sw::ecs::VelocityComponent>();
		// speed по умолчанию = 1

		// Добавляем поведения (в порядке приоритета)
		auto& behaviour = swordsmanEntity->AddComponent<sw::ecs::BehaviourComponent>();
		behaviour.behaviours.push_back(std::make_unique<sw::ecs::MeleeAttack>());
		behaviour.behaviours.push_back(std::make_unique<sw::ecs::MoveToTarget>());

		// Завершаем инициализацию (автоматически занимает клетку на карте)
		world.EndEntityInitialization();

		DEBUG_LOG("Created swordsman entity with ID: " << swordsmanEntity->GetId());
	}

	void CommandManager::ProcessSpawnHunter(const sw::io::SpawnHunter& command)
	{
		auto& worldManager = WorldManager::Get();
		auto& world = worldManager.GetWorld();

		DEBUG_LOG("Processing SpawnHunter: id=" << command.unitId
				   << ", pos=(" << command.x << "," << command.y << ")"
				   << ", hp=" << command.hp << ", agility=" << command.agility
				   << ", strength=" << command.strength << ", range=" << command.range);

		// Создаем сущность охотника
		auto hunterEntity = world.BeginEntityInitialization();

		// Добавляем компоненты
		auto& position = hunterEntity->AddComponent<sw::ecs::PositionComponent>();
		position.x = command.x;
		position.y = command.y;

		auto& health = hunterEntity->AddComponent<sw::ecs::HealthComponent>();
		health.health = command.hp;

		// Добавляем компонент живости (сущность жива)
		hunterEntity->AddComponent<sw::ecs::AliveComponent>();

		auto& strength = hunterEntity->AddComponent<sw::ecs::StrengthComponent>();
		strength.strength = command.strength;

		auto& agility = hunterEntity->AddComponent<sw::ecs::AgilityComponent>();
		agility.agility = command.agility;

		auto& range = hunterEntity->AddComponent<sw::ecs::RangeComponent>();
		range.range = command.range;

		hunterEntity->AddComponent<sw::ecs::ExternalIdComponent>(command.unitId);


		auto& velocity = hunterEntity->AddComponent<sw::ecs::VelocityComponent>();
		// speed по умолчанию = 1

		// Добавляем поведения (в порядке приоритета: дальняя атака, ближняя атака, движение)
		auto& behaviour = hunterEntity->AddComponent<sw::ecs::BehaviourComponent>();
		behaviour.behaviours.push_back(std::make_unique<sw::ecs::RangeAttack>());
		behaviour.behaviours.push_back(std::make_unique<sw::ecs::MeleeAttack>());
		behaviour.behaviours.push_back(std::make_unique<sw::ecs::MoveToTarget>());

		// Завершаем инициализацию (автоматически занимает клетку на карте)
		world.EndEntityInitialization();

		DEBUG_LOG("Created hunter entity with ID: " << hunterEntity->GetId());
	}

	void CommandManager::ProcessMarch(const sw::io::March& command)
	{
		auto& worldManager = WorldManager::Get();
		auto& world = worldManager.GetWorld();

		DEBUG_LOG("Processing March: unitId=" << command.unitId
				   << ", target=(" << command.targetX << "," << command.targetY << ")");

		// Находим сущность по externalId
		sw::ecs::Entity* targetEntity = world.GetEntityByExternalId(command.unitId);

		if (!targetEntity)
		{
			DEBUG_LOG("Warning: Entity with unitId " << command.unitId << " not found");
			return;
		}

		// Добавляем или обновляем MovementTargetComponent
		auto* movementTarget = targetEntity->GetComponent<sw::ecs::MovementTargetComponent>();
		if (!movementTarget)
		{
			auto& newMovementTarget = targetEntity->AddComponent<sw::ecs::MovementTargetComponent>();
			newMovementTarget.targetX = command.targetX;
			newMovementTarget.targetY = command.targetY;
		}
		else
		{
			movementTarget->targetX = command.targetX;
			movementTarget->targetY = command.targetY;
		}

		DEBUG_LOG("Set movement target for entity " << targetEntity->GetId());
	}
}


