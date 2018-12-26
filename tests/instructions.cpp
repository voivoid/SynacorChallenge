#include "boost/test/unit_test.hpp"

#include "synacor/instructions.h"
#include "synacor/memory_storage.h"
#include "synacor/stack.h"

struct InstructionsFixture
{
  synacor::MemoryStorage memory;
  synacor::Stack stack;
};

using namespace synacor;

BOOST_FIXTURE_TEST_CASE( synacor_instructions_set, InstructionsFixture )
{
  memory.store( memory.get_register( 1 ), 42 );

  {
    const Word to( memory.get_register( 0 ) );
    const Word from( memory.get_register( 1 ) );
    synacor::instructions::Set instruction{ to, from };
    instruction.execute( memory, stack );
    BOOST_CHECK_EQUAL( 42, memory.read( memory.get_register( 0 ) ) );
  }

  {
    const Word to( memory.get_register( 0 ) );
    const Word from( 100 );
    synacor::instructions::Set instruction{ to, from };
    instruction.execute( memory, stack );
    BOOST_CHECK_EQUAL( 100, memory.read( memory.get_register( 0 ) ) );
  }
}
