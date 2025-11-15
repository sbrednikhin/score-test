#pragma once

#include <memory>
#include "WorldManager.hpp"
#include "CommandManager.hpp"
#include "ManagerBase.hpp"
#include "ICommandSource.hpp"
#include "ECS/IExternalEventSystem.hpp"

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

        // Получить текущий номер тика
        int GetCurrentTick() const { return _updateCounter; }

        // Работа с источником команд
        void SetCommandSource(std::shared_ptr<ICommandSource> source);
        void ProcessCommands();

        // Статический интерфейс доступа к менеджерам
        static CommandManager& GetCommandManager() { return CommandManager::Get(); }
        static WorldManager& GetWorldManager() { return WorldManager::Get(); }

        // Работа с системой внешних событий
        void SetExternalEventSystem(std::unique_ptr<IExternalEventSystem> eventSystem);
        IExternalEventSystem* GetExternalEventSystem() const { return _externalEventSystem.get(); }

    private:
        std::unique_ptr<WorldManager> _worldManager;
        std::unique_ptr<CommandManager> _commandManager;
        std::weak_ptr<ICommandSource> _commandSource;
        std::unique_ptr<IExternalEventSystem> _externalEventSystem;

        bool _isInitialized;
        bool _isRunning;
        int _updateCounter;
    };
}
