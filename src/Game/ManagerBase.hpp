#pragma once

#include <type_traits>

namespace sw
{
    class ManagerBase
    {
    protected:
        ManagerBase();
        virtual ~ManagerBase();

    public:
        // Запрещаем копирование и перемещение
        ManagerBase(const ManagerBase&) = delete;
        ManagerBase& operator=(const ManagerBase&) = delete;
        ManagerBase(ManagerBase&&) = delete;
        ManagerBase& operator=(ManagerBase&&) = delete;

        // Интерфейс для наследников
        virtual void Initialize() = 0;
        virtual void Deinitialize() = 0;
        virtual void Update() = 0;

        // Статический интерфейс доступа
        template<typename T>
        static T& Get();

        // Проверка инициализации
        static bool IsInitialized() { return _instance != nullptr; }

    private:
        static ManagerBase* _instance;
    };

    // Шаблонная реализация для получения конкретного менеджера
    template<typename T>
    T& ManagerBase::Get()
    {
        static_assert(std::is_base_of_v<ManagerBase, T>, "T must inherit from ManagerBase");
        return static_cast<T&>(*_instance);
    }
}
