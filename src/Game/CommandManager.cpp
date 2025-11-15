#include "CommandManager.hpp"
#include "MapManager.hpp"
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
        // Обрабатываем все команды из очереди
        for (const auto& command : _commands)
        {
            // Определяем тип команды по имени и вызываем соответствующий обработчик
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

        // Очищаем очередь после обработки
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
        // Получаем доступ к MapManager и создаем карту
        auto& mapManager = ManagerBase::Get<MapManager>();
        std::cout << "Processing CreateMap: width=" << command.width << ", height=" << command.height << std::endl;

        // Здесь должна быть логика создания карты в MapManager
        // Пока просто выводим информацию
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
