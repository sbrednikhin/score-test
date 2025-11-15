#pragma once

#include "ICommand.hpp"
#include <cstdint>
#include <iosfwd>

namespace sw::io
{
	struct SpawnSwordsman : ICommand
	{
		constexpr static const char* TypeName = "SPAWN_SWORDSMAN";

		uint32_t unitId{};
		uint32_t x{};
		uint32_t y{};
		uint32_t hp{};
		uint32_t strength{};

		SpawnSwordsman() = default;
		SpawnSwordsman(const SpawnSwordsman& other) :
			unitId(other.unitId), x(other.x), y(other.y), hp(other.hp), strength(other.strength) {}

		virtual const char* GetTypeName() const override { return TypeName; }
		virtual size_t GetTypeId() const override { return std::hash<std::string>{}(TypeName); }

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
			visitor.visit("x", x);
			visitor.visit("y", y);
			visitor.visit("hp", hp);
			visitor.visit("strength", strength);
		}
	};
}
