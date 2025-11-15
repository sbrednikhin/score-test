#pragma once

#include <memory>
#include <vector>
#include <IO/Commands/ICommand.hpp>
#include <IO/System/PrintDebug.hpp>

namespace sw
{
    class CommandManager
    {
    public:
        CommandManager();
        ~CommandManager();

        void Initialize();
        void Deinitialize();

        // Интерфейс управления командами
        void AddCommand(std::shared_ptr<sw::io::ICommand> command);
        const std::vector<std::shared_ptr<sw::io::ICommand>>& GetCommands() const;
        void ClearCommands();

    private:
        std::vector<std::shared_ptr<sw::io::ICommand>> _commands;
    };
}
