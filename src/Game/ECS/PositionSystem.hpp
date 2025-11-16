#pragma once

#include "SystemBase.hpp"

namespace sw::ecs
{
	class PositionSystem : public ISystem
	{
	public:
		PositionSystem() = default;
		~PositionSystem() override = default;

		void ProcessWorldPhase(World& world, UpdatePhase phase) override;

	private:
		void ProcessNewEntities(World& world, class MapService* mapService);
		void ProcessEntitiesForDestruction(World& world, class MapService* mapService);
	};
}
