# Анализ проекта sw_battle_test

## 1. Общее описание проекта

Проект `sw_battle_test` представляет собой C++ реализацию пошаговой симуляции битвы юнитов на карте. Это консольное приложение, которое читает сценарии команд из файла и выводит события симуляции в stdout.

### Цель проекта
- Демонстрация навыков проектирования ПО
- Создание расширяемой архитектуры для симуляции битвы
- Реализация системы команд и событий для управления симуляцией

### Основные компоненты
- **Команды**: CREATE_MAP, SPAWN_SWORDSMAN, SPAWN_HUNTER, MARCH
- **События**: MAP_CREATED, UNIT_SPAWNED, UNIT_DIED, UNIT_ATTACKED, UNIT_MOVED, MARCH_STARTED, MARCH_ENDED
- **Юниты**: Мечник (Swordsman), Охотник (Hunter)

## 2. Архитектура проекта

### Структура директорий
```
src/
├── IO/
│   ├── Commands/          # Структуры команд
│   │   ├── CreateMap.hpp
│   │   ├── March.hpp
│   │   ├── SpawnHunter.hpp
│   │   └── SpawnSwordsman.hpp
│   ├── Events/            # Структуры событий
│   │   ├── MapCreated.hpp
│   │   ├── MarchEnded.hpp
│   │   ├── MarchStarted.hpp
│   │   ├── UnitAttacked.hpp
│   │   ├── UnitDied.hpp
│   │   ├── UnitMoved.hpp
│   │   └── UnitSpawned.hpp
│   └── System/            # Система ввода-вывода
│       ├── CommandParser.hpp/cpp
│       ├── EventLog.hpp
│       ├── PrintDebug.hpp
│       └── details/
│           ├── CommandParserVisitor.hpp
│           └── PrintFieldVisitor.hpp
└── main.cpp
```

### Архитектурные паттерны

#### 1. Visitor Pattern
Используется для унификации работы с командами и событиями:
```cpp
template <typename Visitor>
void visit(Visitor& visitor) {
    visitor.visit("unitId", unitId);
    visitor.visit("unitType", unitType);
    visitor.visit("x", x);
    visitor.visit("y", y);
}
```

#### 2. Command Pattern
Команды инкапсулируют действия системы:
- `CreateMap` - создание карты
- `SpawnSwordsman` - создание мечника
- `SpawnHunter` - создание охотника
- `March` - перемещение юнита

#### 3. Observer/Event Pattern
Система событий для логирования происходящего:
- `EventLog` - централизованный логгер событий
- Структурированные события с метаданными

#### 4. Factory Pattern (неявный)
Через систему парсинга команд создаются соответствующие структуры данных.

## 3. Система команд и событий

### Команды
Все команды наследуют единому интерфейсу через паттерн Visitor:

```cpp
struct SpawnSwordsman {
    constexpr static const char* Name = "SPAWN_SWORDSMAN";
    uint32_t unitId{};
    uint32_t x{}, y{};
    uint32_t hp{};
    uint32_t strength{};

    template <typename Visitor>
    void visit(Visitor& visitor) {
        visitor.visit("unitId", unitId);
        visitor.visit("x", x);
        visitor.visit("y", y);
        visitor.visit("hp", hp);
        visitor.visit("strength", strength);
    }
};
```

### События
Аналогичная структура для событий:

```cpp
struct UnitDied {
    constexpr static const char* Name = "UNIT_DIED";
    uint32_t unitId{};

    template <typename Visitor>
    void visit(Visitor& visitor) {
        visitor.visit("unitId", unitId);
    }
};
```

### Парсер команд
```cpp
class CommandParser {
private:
    std::unordered_map<std::string, std::function<void(std::istream&)>> _commands;

public:
    template <class TCommandData>
    CommandParser& add(std::function<void(TCommandData)> handler) {
        // Регистрация обработчика команды
    }

    void parse(std::istream& stream);
};
```

## 4. Система парсинга и логгирования

### Парсинг команд
- **CommandParserVisitor**: Парсит поля команд из входного потока
- **CommandParser**: Управляет парсингом и вызовом обработчиков
- Поддержка комментариев (строки, начинающиеся с //)
- Игнорирование пустых строк

### Логгирование событий
- **PrintFieldVisitor**: Форматирует поля событий для вывода
- **EventLog**: Логгер с поддержкой временных меток
- Формат вывода: `[tick] EVENT_NAME field1=value1 field2=value2`

## 5. Преимущества и недостатки архитектуры

### Преимущества

#### 1. Высокая расширяемость
- Добавление новых команд: создать структуру с методом visit
- Добавление новых событий: аналогично командам
- Добавление новых юнитов: расширение системы команд

#### 2. Четкое разделение ответственности
- IO слой отделен от игровой логики
- Команды отдельно от событий
- Парсинг отдельно от обработки

#### 3. Type Safety
- Использование constexpr static для имен команд/событий
- Строгая типизация полей структур

#### 4. Простота тестирования
- Команды и события - plain data structures
- Visitor pattern позволяет легко мокать зависимости

#### 5. Читаемость кода
- Явные имена полей
- Единообразная структура всех команд/событий

### Недостатки

#### 1. Отсутствие игровой логики
Проект содержит только скелет системы команд/событий, но не реализует:
- Карту и позиционирование юнитов
- Логику перемещения
- Систему боя
- Управление ходов

#### 2. Недостаточная гибкость Visitor
- Visitor требует знания всех полей на этапе компиляции
- Сложно добавить условную логику в visit методы

#### 3. Отсутствие валидации данных
- Нет проверки корректности входных данных
- Нет бизнес-правил валидации

#### 4. Жесткая связанность формата ввода-вывода
- Формат команд жестко зафиксирован
- Сложно изменить формат вывода событий

## 6. Рекомендации по улучшению

### 1. Реализация игровой логики
```cpp
// Добавить классы для сущностей
class Unit {
    uint32_t id;
    uint32_t hp;
    Position pos;
    virtual void act(GameState& state) = 0;
};

class Swordsman : public Unit {
    uint32_t strength;
    void act(GameState& state) override;
};
```

### 2. Система состояний игры
```cpp
class GameState {
    Map map;
    std::vector<std::unique_ptr<Unit>> units;
    EventLog& logger;
    uint64_t currentTick = 0;

    void processCommands(const std::vector<Command>& commands);
    void executeTurn();
};
```

### 3. Улучшение Visitor паттерна
```cpp
// Добавить reflection-like возможности
template <typename T>
struct FieldDescriptor {
    std::string name;
    T value;
};

// Или использовать более гибкий подход
class ReflectiveVisitor {
    virtual void visit(const std::string& fieldName, auto& value) = 0;
};
```

### 4. Валидация данных
```cpp
class CommandValidator {
public:
    static bool validate(const CreateMap& cmd);
    static bool validate(const SpawnSwordsman& cmd, const GameState& state);
};
```

### 5. Конфигурационные файлы
- Переместить имена команд/событий в конфигурацию
- Сделать формат вывода настраиваемым

### 6. Тестирование
```cpp
// Unit tests для компонентов
TEST(CommandParserTest, ParseCreateMap) {
    // Тест парсинга команды
}

TEST(EventLogTest, LogUnitDied) {
    // Тест логирования события
}
```

### 7. Документация и примеры
- Добавить Doxygen комментарии
- Создать больше примеров сценариев
- Документировать формат команд

## 7. Заключение

Проект демонстрирует хорошую архитектурную основу для пошаговой симуляции битвы с применением современных паттернов C++. Реализованная система команд и событий обеспечивает высокую расширяемость и позволяет легко добавлять новые типы юнитов, действий и характеристик.

Ключевые преимущества архитектуры:
- Четкое разделение ответственности между компонентами
- Высокая расширяемость через паттерн Visitor
- Простота добавления новых команд и событий
- Четкая структура проекта

Рекомендуется продолжить развитие в направлении полноценной игровой логики с реализацией всех описанных в README механик взаимодействия юнитов.
