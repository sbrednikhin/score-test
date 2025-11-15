#include "GameLogic.hpp"

namespace sw
{
    GameLogic::GameLogic()
        : _isInitialized(false)
        , _isRunning(false)
        , _updateCounter(0)
    {
    }

    GameLogic::~GameLogic()
    {
    }

    void GameLogic::Initialize()
    {
        if (_isInitialized) return;

        // Создаем менеджеры
        _mapManager = std::make_unique<MapManager>();
        _commandManager = std::make_unique<CommandManager>();

        // Инициализируем менеджеры
        _mapManager->Initialize();
        _commandManager->Initialize();

        _isInitialized = true;
        _isRunning = true;
        _updateCounter = 0;
    }

    void GameLogic::Deinitialize()
    {
        if (!_isInitialized) return;

        _isRunning = false;

        // Деинициализируем менеджеры
        if (_commandManager) _commandManager->Deinitialize();
        if (_mapManager) _mapManager->Deinitialize();

        // Уничтожаем менеджеры
        _commandManager.reset();
        _mapManager.reset();

        _isInitialized = false;
    }

    void GameLogic::Update()
    {
        if (!_isRunning)
        {
            return;
        }
        ++_updateCounter;

        ProcessCommands();

        // Обновляем менеджеры в правильном порядке
        if (_commandManager) _commandManager->Update();  // Обрабатываем команды
        if (_mapManager) _mapManager->Update();          // Обновляем карту
    }

    bool GameLogic::IsCompleted() const
    {
        // Для демонстрации: выполняем 5 обновлений, затем останавливаемся
        return _isRunning && _updateCounter >= 5;
    }

    void GameLogic::SetCommandSource(std::unique_ptr<ICommandSource> source)
    {
        _commandSource = std::move(source);
    }

    void GameLogic::ProcessCommands()
    {
        if (_commandSource && _commandSource->IsAvailable()) {
            _commandSource->ProcessCommands();
        }
    }
}
