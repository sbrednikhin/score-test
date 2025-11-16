#pragma once

#include <Vec2.hpp>
#include <cstdint>

namespace sw::io
{
	struct MarchStarted
	{
		constexpr static const char* Name = "MARCH_STARTED";

		uint32_t unitId{};
		sw::Vec2 position{};
		sw::Vec2 target{};

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
			visitor.visit("x", position.x);
			visitor.visit("y", position.y);
			visitor.visit("targetX", target.x);
			visitor.visit("targetY", target.y);
		}
	};
}