#include "synacor/synacor.h"
#include "synacor/memory_storage.h"
#include "synacor/stack.h"
#include "synacor/machine.h"
#include <iostream>

namespace synacor
{
    void execute( std::byte* /*code*/ )
    {
        MemoryStorage memory;
        Stack stack;
        Address address{ 0 };

        Machine machine{ memory, stack, address, std::cin, std::cout };
        execute( machine );
    }
}
