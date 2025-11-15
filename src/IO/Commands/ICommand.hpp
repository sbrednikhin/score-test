#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>

namespace sw::io
{
    struct ICommand
    {
        ICommand() = default;
        virtual ~ICommand() = default;

        virtual const char* GetTypeName() const = 0;
        virtual size_t GetTypeId() const = 0;
    };
}
