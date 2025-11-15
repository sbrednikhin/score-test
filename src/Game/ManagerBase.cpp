#include "ManagerBase.hpp"

namespace sw
{
    ManagerBase* ManagerBase::_instance = nullptr;

    ManagerBase::ManagerBase()
    {
        // При создании устанавливаем себя как текущий экземпляр
        _instance = this;
    }

    ManagerBase::~ManagerBase()
    {
        // При уничтожении очищаем указатель
        _instance = nullptr;
    }
}
