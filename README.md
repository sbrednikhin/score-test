# sw_battle_test

Пошаговая симуляция битвы юнитов на карте с использованием архитектуры ECS (Entity-Component-System).

## Сборка

### update_vs_project.bat

Запустите `update_vs_project.bat` для генерации/обновления проекта Visual Studio.

Генерирует/обновляет проект CMake для VS 2022 x64, собирает в Debug, настраивает аргументы (`commands_example.txt`), рабочую директорию, проверяет исключения C++.

### Ручная сборка (CMake)

```bash
# Сборка проекта
mkdir build
cd build
cmake ..
cmake --build . --config Debug

# Запуск с файлом команд
./sw_battle_test.exe ../commands_example.txt
```

### Отладка и логирование

```bash
# Включение DEBUG логов (в CMakeLists.txt раскомментировать)
# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#     add_definitions(-DDEBUG_BUILD=1)
# endif()

# Запуск с логами ECS систем
./sw_battle_test.exe commands_example.txt 2>&1 | grep "PositionSystem\|BehaviourSystem\|DeathSystem"
```

## Структура проекта

```
src/
├── main.cpp              # Точка входа
├── Game/
│   ├── ECS/              # Entity-Component-System
│   ├── CommandManager.*  # Обработка команд
│   └── WorldManager.*    # Управление миром
├── IO/
│   ├── Commands/         # Структуры команд
│   └── Events/           # Структуры событий
└── Debug.hpp             # Макросы логирования

CMakeLists.txt            # Конфигурация сборки
commands_example.txt      # Пример сценария
```

## Расширение проекта

### Добавление нового юнита

```cpp
// CommandManager.cpp
void CommandManager::ProcessSpawnArcher(const sw::io::SpawnArcher& command) {
    auto& world = WorldManager::Get().GetWorld();
    auto archerEntity = world.BeginEntityInitialization();

    // Компоненты
    auto& position = archerEntity->AddComponent<sw::ecs::PositionComponent>();
    position.x = command.x; position.y = command.y;

    // Поведения
    auto& behaviour = archerEntity->AddComponent<sw::ecs::BehaviourComponent>();
    behaviour.behaviours.push_back(std::make_unique<sw::ecs::RangeAttack>());

    world.EndEntityInitialization();
}
```
Добавьте метод создания в CommandManager и соответствующие компоненты.

### Добавление нового компонента

```cpp
// Components.hpp
struct ManaComponent : Component {
    int32_t mana = 0;
    ComponentType GetType() const override { return ComponentType::Mana; }
};

// Component.hpp - добавить в enum ComponentType
enum class ComponentType {
    // ... existing types ...
    Mana,
    // ...
};
```
Добавьте структуру компонента и зарегистрируйте тип в enum ComponentType.

### Добавление нового поведения

```cpp
// BehaviourImplementations.cpp
class CastSpell : public IBehaviour {
public:
    bool Act(World& world, Entity* entity) override {
        auto* mana = entity->GetComponent<ManaComponent>();
        if (mana && mana->mana > 10) {
            // Логика заклинания
            return true;
        }
        return false;
    }
    const char* GetName() const override { return "CastSpell"; }
};
```
Добавьте класс поведения, наследующий IBehaviour, реализуйте Act() и GetName().

### Добавление новой системы

```cpp
// CombatSystem.hpp
class CombatSystem : public ISystem {
public:
    void ProcessWorld(World& world) override {
        // Логика обработки боя
        auto entities = world.GetEntitiesWith<HealthComponent, AliveComponent>();
        for (auto* entity : entities) {
            // Обработка повреждений и т.д.
        }
    }
};

// WorldManager.cpp
_world->RegisterSystem(std::make_unique<ecs::CombatSystem>());
```
Добавьте класс системы, наследующий ISystem, реализуйте ProcessWorld() и зарегистрируйте в WorldManager.

### Добавление нового сервиса

```cpp
// AIService.hpp
class AIService : public ServiceBase {
public:
    explicit AIService(const World& world) : ServiceBase(world) {}
    bool IsReady() const override { return true; }

    void CalculatePath(Entity* entity, int32_t targetX, int32_t targetY) {
        // Логика расчета пути
    }
};

// WorldManager.cpp
_world->RegisterService(std::make_unique<ecs::AIService>(*_world));
```
Добавьте класс сервиса, наследующий ServiceBase, реализуйте IsReady() и зарегистрируйте в WorldManager.