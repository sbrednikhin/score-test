#pragma once

#include <memory>
#include <string>

namespace sw::io { struct ICommand; }

namespace sw
{
    class ICommandSource
    {
    public:
        virtual ~ICommandSource() = default;

        // Основной метод для обработки команд из источника
        virtual void ProcessCommands() = 0;

        // Проверка доступности источника
        virtual bool IsAvailable() const = 0;
    };
}
