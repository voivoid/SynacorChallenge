#include "synacor/machine.h"
#include "synacor/instructions_base.h"
#include "synacor/memory_storage.h"

namespace synacor
{
void run( Machine& machine )
{
  Address current_address{ 0 };
  while ( is_valid_address( current_address ) )
  {
    const auto instruction  = read_instruction( *machine.memory, current_address );
    current_address = instruction->execute( machine, current_address );
  }
}
}  // namespace synacor
