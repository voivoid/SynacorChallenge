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
  }

  template <typename Instruction, typename... Args>
  synacor::Address exec( Args... args )
  {
    return Instruction{ args... }.execute( memory, stack, instruction_addr );
  }

  bool check_result( const synacor::Word expected )
  {
    return expected == memory.read( result_reg );
  }

  template <typename Instruction>
  auto get_arithm_test_func()
  {
    const auto test = [this]( const synacor::Word from1, const synacor::Word from2, const synacor::Word expected ) {
      const auto next_addr = exec<Instruction>( synacor::Word( result_reg ), from1, from2 );

      BOOST_CHECK( check_result( expected ) );
      BOOST_CHECK( next_addr == instruction_addr + synacor::Address( 4 ) );
    };

    return test;
  }

  synacor::MemoryStorage memory;
  synacor::Stack stack;

  synacor::Address result_reg       = memory.get_register( 0 );
  synacor::Address reg_with_42_num  = memory.get_register( 1 );
  synacor::Address instruction_addr = synacor::Address{ 30000 };
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

  const auto test = [this]( const Word from, const Word expected ) {
    const Word to{ result_reg };
    const auto next_addr = exec<synacor::instructions::Set>( to, from );

    BOOST_CHECK( next_addr == instruction_addr + Address( 3 ) );
    BOOST_CHECK( check_result( expected ) );
  };

  test( Word( reg_with_42_num ), 42 );
  test( 100, 100 );
}

// PUSH

BOOST_FIXTURE_TEST_CASE( synacor_instructions_push, InstructionsFixture )
{
  CHECK_MEMORY_IS_NOT_CHANGED;

  const auto test = [this]( const Word from, const Word expected ) {
    const auto next_addr = exec<synacor::instructions::Push>( from );

    BOOST_REQUIRE( !stack.is_empty() );
    BOOST_CHECK_EQUAL( stack.top(), expected );
    BOOST_CHECK( next_addr == instruction_addr + Address( 2 ) );
  };

  test( Word( reg_with_42_num ), 42 );
  test( 100, 100 );
}

// POP

BOOST_FIXTURE_TEST_CASE( synacor_instructions_pop, InstructionsFixture )
{
  BOOST_CHECK( stack.is_empty() );
  stack.push( 42 );

  const auto next_addr = exec<synacor::instructions::Pop>( Word( result_reg ) );
  BOOST_CHECK( check_result( 42 ) );

  BOOST_CHECK( stack.is_empty() );
  BOOST_CHECK( next_addr == instruction_addr + Address( 2 ) );
}

// EQ
BOOST_FIXTURE_TEST_CASE( synacor_instructions_eq, InstructionsFixture )
{
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = [this]( const Word from1, const Word from2, const Word expected ) {
    const auto next_addr = exec<synacor::instructions::Eq>( Word( result_reg ), from1, from2 );

    BOOST_CHECK( check_result( expected ) );
    BOOST_CHECK( next_addr == instruction_addr + Address( 4 ) );
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

  const auto test = [this]( const Word from1, const Word from2, const Word expected ) {
    const auto next_addr = exec<synacor::instructions::Gt>( Word( result_reg ), from1, from2 );

    BOOST_CHECK( check_result( expected ) );
    BOOST_CHECK( next_addr == instruction_addr + Address( 4 ) );
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

  const auto next_addr = exec<synacor::instructions::Jmp>( Word( 1234 ) );
  BOOST_CHECK( next_addr == Address( 1234 ) );
}

// JT
BOOST_FIXTURE_TEST_CASE( synacor_instructions_jt, InstructionsFixture )
{
  CHECK_MEMORY_IS_NOT_CHANGED;
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = [this]( const Word from, const Address expected ) {
    const auto next_addr = exec<synacor::instructions::Jt>( from, Word( 1234 ) );

    BOOST_CHECK( next_addr == expected );
  };

  test( Word( reg_with_42_num ), Address( 1234 ) );
  test( 0, instruction_addr + Address( 3 ) );
}

// JF
BOOST_FIXTURE_TEST_CASE( synacor_instructions_jf, InstructionsFixture )
{
  CHECK_MEMORY_IS_NOT_CHANGED;
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = [this]( const Word from, const Address expected ) {
    const auto next_addr = exec<synacor::instructions::Jf>( from, Word( 1234 ) );

    BOOST_CHECK( next_addr == expected );
  };

  test( 0, Address( 1234 ) );
  test( Word( reg_with_42_num ), instruction_addr + Address( 3 ) );
}

// ADD
BOOST_FIXTURE_TEST_CASE( synacor_instructions_add, InstructionsFixture )
{
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = get_arithm_test_func<synacor::instructions::Add>();
  test( Word( reg_with_42_num ), Word( reg_with_42_num ), 84 );
  test( 100, 200, 300 );
  test( 32758, 15, 5 );
}

// MULT
BOOST_FIXTURE_TEST_CASE( synacor_instructions_mult, InstructionsFixture )
{
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = get_arithm_test_func<synacor::instructions::Mult>();
  test( Word( reg_with_42_num ), Word( reg_with_42_num ), 1764 );
  test( 100, 200, 20000 );
  test( 32758, 15, 32618 );
}

// MOD
BOOST_FIXTURE_TEST_CASE( synacor_instructions_mod, InstructionsFixture )
{
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = get_arithm_test_func<synacor::instructions::Mod>();
  test( Word( reg_with_42_num ), Word( reg_with_42_num ), 0 );
  test( 100, 200, 100 );
  test( 32758, 15, 13 );
}

// AND
BOOST_FIXTURE_TEST_CASE( synacor_instructions_and, InstructionsFixture )
{
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = get_arithm_test_func<synacor::instructions::And>();
  test( Word( reg_with_42_num ), Word( reg_with_42_num ), 42 );
  test( 100, 200, 64 );
  test( 32758, 15, 6 );
}

// OR
BOOST_FIXTURE_TEST_CASE( synacor_instructions_or, InstructionsFixture )
{
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = get_arithm_test_func<synacor::instructions::Or>();
  test( Word( reg_with_42_num ), Word( reg_with_42_num ), 42 );
  test( 100, 200, 236 );
  test( 32758, 15, 32767 );
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

  const auto test = [&ss, this]( const Word from, const char expected ) {
    const auto next_addr = exec<synacor::instructions::Out>( from );

    BOOST_CHECK_EQUAL( expected, ss.get() );
    BOOST_CHECK( next_addr == instruction_addr + Address( 2 ) );
  };

  test( Word( reg_with_42_num ), '*' );
  test( 100, 'd' );
}

// NOOP

BOOST_FIXTURE_TEST_CASE( synacor_instructions_noop, InstructionsFixture )
{
  CHECK_MEMORY_IS_NOT_CHANGED;
  CHECK_STACK_IS_NOT_CHANGED;

  const auto next_addr = exec<synacor::instructions::Noop>();
  BOOST_CHECK( next_addr == instruction_addr + Address( 1 ) );
}
