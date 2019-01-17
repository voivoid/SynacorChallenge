#pragma once

#include "synacor/arch.h"

namespace synacor
{
    struct IMemoryReader
    {
        virtual ~IMemoryReader() = default;
        virtual Word read( Address address ) const = 0;
    };

    struct IMemory : IMemoryReader
    {
        virtual void store( Address address, Word number ) = 0;
    };

    struct IStack
    {
        virtual ~IStack() = default;
        virtual void push( Number word ) = 0;
        virtual Number pop() = 0;
        virtual bool is_empty() const = 0;
    };

    struct IStream
    {
        virtual ~IStream() = default;
        virtual void put( char c ) = 0;
        virtual char get() = 0;
    };
}
