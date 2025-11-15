#include "CommandManager.hpp"
#include "WorldManager.hpp"
#include "ECS/Components.hpp"
#include <iostream>
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
            std::cout << command->GetTypeName() << " command added (ID: " << command->GetTypeId() << ")" << std::endl;
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
        auto& worldManager = ManagerBase::Get<WorldManager>();
        auto& world = worldManager.GetWorld();

        std::cout << "Processing CreateMap: width=" << command.width << ", height=" << command.height << std::endl;

        // Создаем и инициализируем сущность с компонентом карты
        auto mapEntity = world.BeginEntityInitialization();
        auto& mapComponent = mapEntity->AddComponent<sw::ecs::MapComponent>();
        mapComponent.width = command.width;
        mapComponent.height = command.height;

        // Завершаем инициализацию сущности
        world.EndEntityInitialization();

        std::cout << "Created map entity with ID: " << mapEntity->GetId() << std::endl;
    }

    void CommandManager::ProcessSpawnSwordsman(const sw::io::SpawnSwordsman& command)
    {
        std::cout << "Processing SpawnSwordsman: id=" << command.unitId
                  << ", pos=(" << command.x << "," << command.y << ")"
                  << ", hp=" << command.hp << ", strength=" << command.strength << std::endl;

        // Здесь должна быть логика создания мечника
    }

    void CommandManager::ProcessSpawnHunter(const sw::io::SpawnHunter& command)
    {
        std::cout << "Processing SpawnHunter: id=" << command.unitId
                  << ", pos=(" << command.x << "," << command.y << ")"
                  << ", hp=" << command.hp << ", agility=" << command.agility
                  << ", strength=" << command.strength << ", range=" << command.range << std::endl;

        // Здесь должна быть логика создания охотника
    }

    void CommandManager::ProcessMarch(const sw::io::March& command)
    {
        std::cout << "Processing March: unitId=" << command.unitId
                  << ", target=(" << command.targetX << "," << command.targetY << ")" << std::endl;

        // Здесь должна быть логика перемещения юнита
    }
}
