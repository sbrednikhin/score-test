#pragma once

#include <Vec2.hpp>
#include <cstdint>
#include <string>

namespace sw::io
{
	struct UnitMoved
	{
		constexpr static const char* Name = "UNIT_MOVED";

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