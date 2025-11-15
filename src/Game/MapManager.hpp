#pragma once

#include "ManagerBase.hpp"

namespace sw
{
    class MapManager : public ManagerBase
    {
    public:
        MapManager();
        ~MapManager();

        void Initialize() override;
        void Deinitialize() override;
        void Update() override;
    };
}
