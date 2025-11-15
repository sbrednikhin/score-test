#include "GameLogic.hpp"

namespace sw
{
    GameLogic::GameLogic()
        : _isRunning(false)
        , _updateCounter(0)
    {
    }

    GameLogic::~GameLogic()
    {
    }

    void GameLogic::Initialize()
    {
        _mapManager.Initialize();
        _commandManager.Initialize();

        _isRunning = true;
        _updateCounter = 0;
    }

    void GameLogic::Deinitialize()
    {
        _isRunning = false;

        _commandManager.Deinitialize();
        _mapManager.Deinitialize();
    }

    void GameLogic::Update()
    {
        if (!_isRunning)
        {
            return;
        }

        ++_updateCounter;

        // Обновляем менеджеры
        _mapManager.Update();
        // CommandManager не имеет Update метода
    }

    bool GameLogic::IsCompleted() const
    {
        // Для демонстрации: выполняем 5 обновлений, затем останавливаемся
        return _isRunning && _updateCounter >= 5;
    }
}
