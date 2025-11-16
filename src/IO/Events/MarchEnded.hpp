#pragma once

#include <Vec2.hpp>
#include <cstdint>

namespace sw::io
{
	struct MarchEnded
	{
		constexpr static const char* Name = "MARCH_ENDED";

		uint32_t unitId{};
		sw::Vec2 position{};

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
			visitor.visit("x", position.x);
			visitor.visit("y", position.y);
		}
	};
}