#include "synacor/instructions_base.h"

#include "synacor/memory_storage.h"
#include "synacor/instructions.h"
#include "synacor/assert.h"

#include <type_traits>

namespace
{


    template <typename Cmd>
    std::unique_ptr<synacor::Instruction> make_instruction( const synacor::MemoryStorage& memory, synacor::Address command_address )
    {

    }
}

namespace synacor
{

std::unique_ptr<Instruction> read_instruction( const MemoryStorage& memory, Address command_address )
{
    const auto cmd_id = memory.read( command_address );
    switch( cmd_id )
    {
    }

    SYNACOR_ENSURE( false );
}

}
