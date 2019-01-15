#include "synacor/synacor.h"
#include "synacor/machine.h"
#include "synacor/memory_storage.h"
#include "synacor/stack.h"
#include <iostream>

namespace
{
    void load_memory( synacor::MemoryStorage& memory, const std::byte* const memory_data, const size_t memory_data_size )
    {
      const size_t memory_data_size_in_words = ( memory_data_size / ( sizeof( synacor::Word ) / sizeof( std::byte ) ) );
      const synacor::Word* const memory_data_begin = reinterpret_cast<const synacor::Word*>(memory_data);
      const synacor::Word* const memory_data_end = memory_data_begin + memory_data_size_in_words;
      memory.load_memory( memory_data_begin, memory_data_end );
    }
}

namespace synacor
{
void execute( const std::byte* const memory_data, const size_t memory_data_size )
{
  Machine machine{ std::make_unique<MemoryStorage>(), std::make_unique<Stack>(), Address{ 0 }, std::cin, std::cout };
  load_memory( *machine.memory, memory_data, memory_data_size );

  run( machine );
}
}  // namespace synacor
