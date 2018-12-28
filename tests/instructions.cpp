#include "boost/test/unit_test.hpp"

#include "synacor/instructions.h"
#include "synacor/memory_storage.h"
#include "synacor/stack.h"

#include "boost/scope_exit.hpp"

#include <sstream>

struct InstructionsFixture
{
  InstructionsFixture()
  {
    memory.store( memory.get_register( 1 ), 42 );

    result_reg      = memory.get_register( 0 );
    reg_with_42_num = memory.get_register( 1 );
  }

  template <typename Instruction, typename... Args>
  void exec( short unsigned int instruction_size, Args... args )
  {
    const synacor::Address instruction_addr{ 30000 };
    const synacor::Address next_instruction_addr = Instruction{ args... }.execute( memory, stack, instruction_addr );
    BOOST_CHECK( next_instruction_addr == instruction_addr + synacor::Address( instruction_size ) );
  }

  void check_result( const synacor::Word expected )
  {
      BOOST_CHECK_EQUAL( expected, memory.read( result_reg ) );
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

    check_result( expected );
  };

  test( Word( reg_with_42_num ), 42 );
  test( 100, 100 );
}

// PUSH

BOOST_FIXTURE_TEST_CASE( synacor_instructions_push, InstructionsFixture )
{
  CHECK_MEMORY_IS_NOT_CHANGED;

  auto test = [this]( const Word from, const Word expected ) {
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
  check_result( 42 );

  BOOST_CHECK( stack.is_empty() );
}

// EQ
BOOST_FIXTURE_TEST_CASE( synacor_instructions_eq, InstructionsFixture )
{
  CHECK_STACK_IS_NOT_CHANGED;

  auto test = [this]( const Word from1, const Word from2, const Word expected ) {
    exec<synacor::instructions::Eq>( 4, Word( result_reg ), from1, from2 );

    check_result( expected );
  };

  test( Word( reg_with_42_num ), Word( reg_with_42_num ), 1 );
  test( Word( reg_with_42_num ), 100, 0 );
  test( 100, Word( reg_with_42_num ), 0 );
  test( 100, 100, 1 );
}

// GT
BOOST_FIXTURE_TEST_CASE( synacor_instructions_gt, InstructionsFixture )
{
  CHECK_STACK_IS_NOT_CHANGED;

  auto test = [this]( const Word from1, const Word from2, const Word expected ) {
    exec<synacor::instructions::Gt>( 4, Word( result_reg ), from1, from2 );

    check_result( expected );
  };

  test( Word( reg_with_42_num ), Word( reg_with_42_num ), 0 );
  test( Word( reg_with_42_num ), 10, 1 );
  test( 100, Word( reg_with_42_num ), 1 );
  test( 100, 100, 0 );
}

// JMP
BOOST_FIXTURE_TEST_CASE( synacor_instructions_jmp, InstructionsFixture )
{
    CHECK_MEMORY_IS_NOT_CHANGED;
    CHECK_STACK_IS_NOT_CHANGED;
}

// JT
BOOST_FIXTURE_TEST_CASE( synacor_instructions_jt, InstructionsFixture )
{
    CHECK_MEMORY_IS_NOT_CHANGED;
    CHECK_STACK_IS_NOT_CHANGED;
}

// JF
BOOST_FIXTURE_TEST_CASE( synacor_instructions_jf, InstructionsFixture )
{
    CHECK_MEMORY_IS_NOT_CHANGED;
    CHECK_STACK_IS_NOT_CHANGED;
}

// ADD

BOOST_FIXTURE_TEST_CASE( synacor_instructions_add, InstructionsFixture )
{
  CHECK_STACK_IS_NOT_CHANGED;

  auto test = [this]( const Word from1, const Word from2, const Word expected ) {
    exec<synacor::instructions::Add>( 4, Word( result_reg ), from1, from2 );

    check_result( expected );
  };

  test( Word( reg_with_42_num ), Word( reg_with_42_num ), 84 );
  test( 100, 200, 300 );
}

// OUT
BOOST_FIXTURE_TEST_CASE( synacor_instructions_out, InstructionsFixture )
{
  CHECK_MEMORY_IS_NOT_CHANGED;
  CHECK_STACK_IS_NOT_CHANGED;

  std::stringstream ss;
  synacor::instructions::set_ostream( &ss );

  BOOST_SCOPE_EXIT( void )
  {
    synacor::instructions::set_ostream( nullptr );
  }
  BOOST_SCOPE_EXIT_END;

  auto test = [&ss, this]( const Word from, const char expected ) {
    exec<synacor::instructions::Out>( 2, from );

    BOOST_CHECK_EQUAL( expected, ss.get() );
  };

  test( Word( reg_with_42_num ), '*' );
  test( 100, 'd' );
}

// NOOP

BOOST_FIXTURE_TEST_CASE( synacor_instructions_noop, InstructionsFixture )
{
  CHECK_MEMORY_IS_NOT_CHANGED;
  CHECK_STACK_IS_NOT_CHANGED;

  exec<synacor::instructions::Noop>( 1 );
}
