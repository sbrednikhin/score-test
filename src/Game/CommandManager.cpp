#include "CommandManager.hpp"
#include <iostream>

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
}
