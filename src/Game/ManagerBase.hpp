#pragma once

#include <type_traits>

namespace sw
{
	template<typename Derived>
	class ManagerBase
	{
	protected:
		ManagerBase()
		{
			// При создании устанавливаем себя как текущий экземпляр
			_instance = static_cast<Derived*>(this);
		}

		virtual ~ManagerBase()
		{
			// При уничтожении очищаем указатель
			_instance = nullptr;
		}

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
		static Derived& Get()
		{
			static_assert(std::is_base_of_v<ManagerBase<Derived>, Derived>, "Derived must inherit from ManagerBase<Derived>");
			return *_instance;
		}

		// Проверка инициализации
		static bool IsInitialized() { return _instance != nullptr; }

	private:
		static Derived* _instance;
	};

	// Статическая переменная для каждого типа
	template<typename Derived>
	Derived* ManagerBase<Derived>::_instance = nullptr;
}


