#pragma once

#include <Vec2.hpp>
#include <cstdint>
#include <string>

namespace sw::io
{
	struct UnitSpawned
	{
		constexpr static const char* Name = "UNIT_SPAWNED";

		uint32_t unitId{};
		std::string unitType{};
		sw::Vec2 position{};

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
			visitor.visit("unitType", unitType);
			visitor.visit("x", position.x);
			visitor.visit("y", position.y);
		}
	};
}