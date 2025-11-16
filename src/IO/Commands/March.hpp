#pragma once

#include "ICommand.hpp"
#include <Vec2.hpp>
#include <cstdint>
#include <iosfwd>

namespace sw::io
{
	struct March : ICommand
	{
		constexpr static const char* TypeName = "MARCH";

		uint32_t unitId{};
		sw::Vec2 target{};

		March() = default;
		March(const March& other) :
			unitId(other.unitId), target(other.target) {}

		virtual const char* GetTypeName() const override { return TypeName; }
		virtual size_t GetTypeId() const override { return std::hash<std::string>{}(TypeName); }

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
			visitor.visit("targetX", target.x);
			visitor.visit("targetY", target.y);
		}
	};
}