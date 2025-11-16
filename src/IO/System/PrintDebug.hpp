#pragma once

#include <IO/System/details/PrintFieldVisitor.hpp>

namespace sw
{
	template <typename TCommand>
	void printDebug(std::ostream& stream, TCommand& data)
	{
		stream << data.GetTypeName() << ' ';
		PrintFieldVisitor visitor(stream);
		data.visit(visitor);
		stream << "\n";
	}
}