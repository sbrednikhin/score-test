#pragma once

#include <memory>
#include <vector>
#include <IO/Commands/ICommand.hpp>
#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>
#include <IO/Commands/SpawnHunter.hpp>
#include <IO/Commands/March.hpp>
#include <IO/System/PrintDebug.hpp>
#include "ManagerBase.hpp"

namespace sw
{
    class CommandManager final : public ManagerBase<CommandManager>
    {
    public:
        CommandManager();
        ~CommandManager();

        void Initialize() override;
        void Deinitialize() override;
        void Update() override;

        // Интерфейс управления командами
        void AddCommand(std::shared_ptr<sw::io::ICommand> command);
        const std::vector<std::shared_ptr<sw::io::ICommand>>& GetCommands() const;
        void ClearCommands();

        // Обработчики команд
        void ProcessCreateMap(const sw::io::CreateMap& command);
        void ProcessSpawnSwordsman(const sw::io::SpawnSwordsman& command);
        void ProcessSpawnHunter(const sw::io::SpawnHunter& command);
        void ProcessMarch(const sw::io::March& command);

    private:
        std::vector<std::shared_ptr<sw::io::ICommand>> _commands;
    };
}
