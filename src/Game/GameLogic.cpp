#include "GameLogic.hpp"
#include "EventLogger.hpp"
#include "ECS/EventLogSystem.hpp"
#include "ECS/WorldHelper.hpp"

namespace sw
{
    using namespace ecs;
    GameLogic::GameLogic(std::shared_ptr<ICommandSource> commandSource)
        : _commandSource(commandSource)
        , _isInitialized(false)
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
        _worldManager = std::make_unique<WorldManager>();
        _commandManager = std::make_unique<CommandManager>();

        // Инициализируем менеджеры
        _worldManager->Initialize();  // Сначала инициализируем мир (без сущностей)
        _commandManager->Initialize();

        // Обрабатываем команды из источника
        if (auto source = _commandSource.lock())
        {
            source->ProcessCommands();
        }

        // Создаем систему событий (теперь безопасно передавать ссылку на себя)
        auto eventSystem = std::make_unique<EventLogSystem>(*this);
        SetExternalEventSystem(std::move(eventSystem));

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
        if (_worldManager) _worldManager->Deinitialize();

        // Уничтожаем менеджеры
        _commandManager.reset();
        _worldManager.reset();

        _isInitialized = false;
    }

    void GameLogic::Update()
    {
        if (!_isRunning)
        {
            return;
        }
        ++_updateCounter;

        // Обновляем менеджеры в правильном порядке
        if (_commandManager) _commandManager->Update();  // Обрабатываем команды
        if (_worldManager) _worldManager->Update();      // Обновляем мир (системы ECS)
    }

    bool GameLogic::IsCompleted() const
    {
        if (!_isRunning) return true;

        // Если есть активные сущности, продолжаем симуляцию
		if (_worldManager && WorldHelper::HasActiveEntities(_worldManager->GetWorld()))
        {
            return false;
        }

        // Если есть необработанные команды, продолжаем симуляцию
        if (_commandManager && !_commandManager->GetCommands().empty())
        {
            return false;
        }

        // Нет активных сущностей и нет команд - завершаемся
        return true;
    }


    void GameLogic::SetExternalEventSystem(std::unique_ptr<IExternalEventSystem> eventSystem)
    {
        _externalEventSystem = std::move(eventSystem);
        if (_externalEventSystem)
        {
            EventLogger::SetEventSystem(_externalEventSystem.get());
        }
        else
        {
            EventLogger::SetEventSystem(nullptr);
        }
    }
}
