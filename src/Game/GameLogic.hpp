#pragma once

#include "MapManager.hpp"
#include "CommandManager.hpp"

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

    public:
        CommandManager& GetCommandManager() { return _commandManager; }

    private:
        MapManager _mapManager;
        CommandManager _commandManager;

        bool _isRunning;
        int _updateCounter;
    };
}
