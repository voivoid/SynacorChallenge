#include "synacor/synacor.h"
#include "synacor/machine.h"
#include "synacor/memory_storage.h"
#include "synacor/stack.h"
#include <iostream>

namespace synacor
{
void execute( std::byte* /*code*/ )
{
  Machine machine{ std::make_unique<MemoryStorage>(), std::make_unique<Stack>(), Address{ 0 }, std::cin, std::cout };
  execute( machine );
}
}  // namespace synacor
