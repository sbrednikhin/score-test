#pragma once

#include "IExternalEventSystem.hpp"
#include "ManagerBase.hpp"

namespace sw
{
	// Менеджер для доступа к системе событий
	class EventSystemManager : public ManagerBase<EventSystemManager>
	{
	public:
		// Конструктор с системой событий
		explicit EventSystemManager(IExternalEventSystem* eventSystem);

		// Интерфейс ManagerBase (пустые реализации, так как менеджер не обновляется)
		void Initialize() override {}
		void Deinitialize() override {}
		void Update() override {}

		// Получение системы событий
		IExternalEventSystem& GetEventSystem() const;

	private:
		IExternalEventSystem* _eventSystem;
	};
}