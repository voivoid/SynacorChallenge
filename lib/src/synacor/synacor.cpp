#include "synacor/synacor.h"
#include "synacor/io.h"
#include "synacor/machine.h"
#include "synacor/memory_storage.h"
#include "synacor/stack.h"
#include <iostream>

namespace
{
void load_memory( synacor::MemoryStorage& memory, const std::byte* const memory_data, const size_t memory_data_size )
{
  const size_t memory_data_size_in_words       = ( memory_data_size / ( sizeof( synacor::Word ) / sizeof( std::byte ) ) );
  const synacor::Word* const memory_data_begin = reinterpret_cast<const synacor::Word*>( memory_data );
  const synacor::Word* const memory_data_end   = memory_data_begin + memory_data_size_in_words;
  memory.load_memory( memory_data_begin, memory_data_end );
}
}  // namespace

namespace synacor
{
void execute( const std::byte* const memory_data, const size_t memory_data_size )
{
  MemoryStorage memory;
  load_memory( memory, memory_data, memory_data_size );

  Stack stack;
  IO io{ std::cin, std::cout };

  Machine machine{ memory, stack, io };
  run( machine );
}
}  // namespace synacor
