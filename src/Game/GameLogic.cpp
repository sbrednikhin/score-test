#include "GameLogic.hpp"
#include "EventLogSystem.hpp"
#include "IExternalEventSystem.hpp"
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

		// Инициализируем систему событий (нужна для логгирования команд)
		InitializeExternalEventSystem();


		// Инициализируем менеджеры
		_worldManager->Initialize();  // Сначала инициализируем мир (без сущностей)
		_commandManager->Initialize();

		// Обрабатываем команды из источника
		if (auto source = _commandSource.lock())
		{
			source->ProcessCommands();
		}

	_isInitialized = true;
	_isRunning = true;
	_updateCounter = 0;
	}

	void GameLogic::InitializeExternalEventSystem()
	{
		_externalEventSystem = std::make_unique<EventLogSystem>(*this);
		_eventSystemManager = std::make_unique<EventSystemManager>(_externalEventSystem.get());
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
		if (_worldManager) _worldManager->Update();	  // Обновляем мир (системы ECS)
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
}