#pragma once

namespace sw
{
    class MapManager
    {
    public:
        MapManager();
        ~MapManager();

        void Initialize();
        void Deinitialize();
        void Update();
    };
}
