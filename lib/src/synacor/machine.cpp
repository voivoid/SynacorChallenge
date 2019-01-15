#include "synacor/machine.h"
#include "synacor/instructions_base.h"
#include "synacor/memory_storage.h"

namespace synacor
{
void run( Machine& machine )
{
  while ( is_valid_address( machine.current_address ) )
  {
    const auto instruction  = read_instruction( *machine.memory, machine.current_address );
    machine.current_address = instruction->execute( machine );
  }
}
}  // namespace synacor
