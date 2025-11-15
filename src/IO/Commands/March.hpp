#pragma once

#include "ICommand.hpp"
#include <cstdint>
#include <iosfwd>

namespace sw::io
{
	struct March : ICommand
	{
		constexpr static const char* TypeName = "MARCH";

		uint32_t unitId{};
		uint32_t targetX{};
		uint32_t targetY{};

		March() = default;
		March(const March& other) :
			unitId(other.unitId), targetX(other.targetX), targetY(other.targetY) {}

		virtual const char* GetTypeName() const override { return TypeName; }
		virtual size_t GetTypeId() const override { return std::hash<std::string>{}(TypeName); }

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
			visitor.visit("targetX", targetX);
			visitor.visit("targetY", targetY);
		}
	};
}
