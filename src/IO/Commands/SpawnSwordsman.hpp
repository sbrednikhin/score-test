#pragma once

#include "ICommand.hpp"
#include <Vec2.hpp>
#include <cstdint>
#include <iosfwd>

namespace sw::io
{
	struct SpawnSwordsman : ICommand
	{
		constexpr static const char* TypeName = "SPAWN_SWORDSMAN";

		uint32_t unitId{};
		sw::Vec2 position{};
		uint32_t hp{};
		uint32_t strength{};

		SpawnSwordsman() = default;
		SpawnSwordsman(const SpawnSwordsman& other) :
			unitId(other.unitId), position(other.position), hp(other.hp), strength(other.strength) {}

		virtual const char* GetTypeName() const override { return TypeName; }
		virtual size_t GetTypeId() const override { return std::hash<std::string>{}(TypeName); }

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
			visitor.visit("x", position.x);
			visitor.visit("y", position.y);
			visitor.visit("hp", hp);
			visitor.visit("strength", strength);
		}
	};
}