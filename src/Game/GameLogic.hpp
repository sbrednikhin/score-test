#pragma once

#include <memory>
#include "WorldManager.hpp"
#include "CommandManager.hpp"
#include "ManagerBase.hpp"
#include "ICommandSource.hpp"

namespace sw
{
    class GameLogic
    {
    public:
        GameLogic();
        ~GameLogic();

        void Initialize();
        void Deinitialize();
        void Update();

        // Метод для определения необходимости следующего update
        bool IsCompleted() const;

        // Метод для проверки инициализации
        bool IsInitialized() const { return _isInitialized; }

        // Работа с источником команд
        void SetCommandSource(std::shared_ptr<ICommandSource> source);
        void ProcessCommands();

        // Статический интерфейс доступа к менеджерам
        static CommandManager& GetCommandManager() { return ManagerBase::Get<CommandManager>(); }
        static WorldManager& GetWorldManager() { return ManagerBase::Get<WorldManager>(); }

    private:
        std::unique_ptr<WorldManager> _worldManager;
        std::unique_ptr<CommandManager> _commandManager;
        std::weak_ptr<ICommandSource> _commandSource;

        bool _isInitialized;
        bool _isRunning;
        int _updateCounter;
    };
}
