#pragma once

#include "SystemBase.hpp"

namespace sw::ecs
{
	class DeathSystem : public ISystem
	{
	public:
		DeathSystem() = default;
		~DeathSystem() override = default;

		void ProcessWorldPhase(World& world, UpdatePhase phase) override;
	};
}