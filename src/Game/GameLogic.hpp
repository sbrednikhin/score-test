#pragma once

#include <memory>
#include "MapManager.hpp"
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
        void SetCommandSource(std::unique_ptr<ICommandSource> source);
        void ProcessCommands();

        // Статический интерфейс доступа к менеджерам
        static CommandManager& GetCommandManager() { return ManagerBase::Get<CommandManager>(); }
        static MapManager& GetMapManager() { return ManagerBase::Get<MapManager>(); }

    private:
        std::unique_ptr<MapManager> _mapManager;
        std::unique_ptr<CommandManager> _commandManager;
        std::unique_ptr<ICommandSource> _commandSource;

        bool _isInitialized;
        bool _isRunning;
        int _updateCounter;
    };
}
