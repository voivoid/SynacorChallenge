#include "synacor/machine.h"
#include "synacor/memory_storage.h"
#include "synacor/instructions_base.h"

namespace synacor
{
    void execute( Machine& machine )
    {
        while( is_valid_address( machine.current_address ) )
        {
           const auto instruction = read_instruction( machine.memory, machine.current_address );
           machine.current_address = instruction->execute( machine );
        }

    }
}
