#pragma once

#include "SystemBase.hpp"

namespace sw::ecs
{
	class BehaviourSystem : public ISystem
	{
	public:
		BehaviourSystem() = default;
		~BehaviourSystem() override = default;

		void ProcessWorld(World& world) override;
	};
}