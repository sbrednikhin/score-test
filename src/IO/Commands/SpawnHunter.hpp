#pragma once

#include "ICommand.hpp"
#include <Vec2.hpp>
#include <cstdint>
#include <iosfwd>

namespace sw::io
{
	struct SpawnHunter : ICommand
	{
		constexpr static const char* TypeName = "SPAWN_HUNTER";

		uint32_t unitId{};
		sw::Vec2 position{};
		uint32_t hp{};
		uint32_t agility{};
		uint32_t strength{};
		uint32_t range{};

		SpawnHunter() = default;
		SpawnHunter(const SpawnHunter& other) :
			unitId(other.unitId), position(other.position), hp(other.hp),
			agility(other.agility), strength(other.strength), range(other.range) {}

		virtual const char* GetTypeName() const override { return TypeName; }
		virtual size_t GetTypeId() const override { return std::hash<std::string>{}(TypeName); }

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
			visitor.visit("x", position.x);
			visitor.visit("y", position.y);
			visitor.visit("hp", hp);
			visitor.visit("agility", agility);
			visitor.visit("strength", strength);
			visitor.visit("range", range);
		}
	};
}