#include "boost/test/unit_test.hpp"

#include "synacor/instructions.h"
#include "synacor/memory_storage.h"
#include "synacor/stack.h"

#include "boost/scope_exit.hpp"

struct InstructionsFixture
{
  InstructionsFixture()
  {
    memory.store( memory.get_register( 1 ), 42 );

    result_reg      = memory.get_register( 0 );
    reg_with_42_num = memory.get_register( 1 );
  }

  template <typename Instruction, typename... Args>
  void exec( short unsigned int instruction_size, Args... args)
  {
      const synacor::Address instruction_addr{ 30000 };
      const synacor::Address next_instruction_addr = Instruction{ args... }.execute( memory, stack, instruction_addr );
      BOOST_CHECK( next_instruction_addr == instruction_addr + synacor::Address( instruction_size ) );
  }

  synacor::MemoryStorage memory;
  synacor::Stack stack;

  synacor::Address result_reg;
  synacor::Address reg_with_42_num;

};

#define CHECK_IS_NOT_CHANGED( var )                                                                                                        \
  BOOST_SCOPE_EXIT( var, this_ )                                                                                                           \
  {                                                                                                                                        \
    BOOST_CHECK( var == this_->var );                                                                                                      \
  }                                                                                                                                        \
  BOOST_SCOPE_EXIT_END

#define CHECK_MEMORY_IS_NOT_CHANGED CHECK_IS_NOT_CHANGED( memory )
#define CHECK_STACK_IS_NOT_CHANGED CHECK_IS_NOT_CHANGED( stack )

using namespace synacor;

// SET

BOOST_FIXTURE_TEST_CASE( synacor_instructions_set, InstructionsFixture )
{
  CHECK_STACK_IS_NOT_CHANGED;

  auto test = [this]( const Word from, const Word expected ) {
     const Word to{ result_reg };
     exec<synacor::instructions::Set>( 3, to, from );

     BOOST_CHECK_EQUAL( expected, memory.read( result_reg ) );
  };

  test( Word( reg_with_42_num ), 42 );
  test( 100, 100 );
}

// PUSH

BOOST_FIXTURE_TEST_CASE( synacor_instructions_push, InstructionsFixture )
{
    CHECK_MEMORY_IS_NOT_CHANGED;

    auto test =[this]( const Word from, const Word expected ) {
        exec<synacor::instructions::Push>( 2, from );

        BOOST_REQUIRE( !stack.is_empty() );
        BOOST_CHECK_EQUAL( stack.top(), expected );
    };

    test( Word( reg_with_42_num ), 42 );
    test( 100, 100 );
}

// POP

BOOST_FIXTURE_TEST_CASE( synacor_instructions_pop, InstructionsFixture )
{
    BOOST_CHECK( stack.is_empty() );
    stack.push( 42 );

    exec<synacor::instructions::Pop>( 2, Word( result_reg ) );
    BOOST_CHECK_EQUAL( memory.read( result_reg ), 42 );

    BOOST_CHECK( stack.is_empty() );
}

// NOOP

BOOST_FIXTURE_TEST_CASE( synacor_instructions_noop, InstructionsFixture )
{
  CHECK_MEMORY_IS_NOT_CHANGED;
  CHECK_STACK_IS_NOT_CHANGED;

  exec<synacor::instructions::Noop>( 1 );
}
