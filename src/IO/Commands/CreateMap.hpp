#pragma once

#include "ICommand.hpp"
#include <cstdint>
#include <iosfwd>
#include <iostream>

namespace sw::io
{
	struct CreateMap : ICommand
	{
		constexpr static const char* TypeName = "CREATE_MAP";

		uint32_t width{};
		uint32_t height{};

		CreateMap() = default;

		virtual const char* GetTypeName() const override { return TypeName; }
		virtual size_t GetTypeId() const override { return std::hash<std::string>{}(TypeName); }

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("width", width);
			visitor.visit("height", height);
		}
	};
}


