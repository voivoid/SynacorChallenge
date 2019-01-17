#include "boost/test/unit_test.hpp"

#include "synacor/instructions.h"
#include "synacor/memory_storage.h"
#include "synacor/stack.h"
#include "synacor/io.h"
#include "test_utils.h"

#include "boost/scope_exit.hpp"

#include <sstream>

#ifdef _MSC_VER
#  pragma warning( disable : 4459 )
#endif

namespace
{

struct InstructionsFixture
{
  InstructionsFixture() : io( io_ss, io_ss )
  {
    memory.store( memory.get_register( 1 ), 42 );
  }

  template <typename Instruction, typename... Args>
  synacor::Address exec( Args... args )
  {
    synacor::Machine machine{ memory, stack, io };
    return Instruction{ args... }.execute( machine, instruction_addr );
  }

  synacor::Word read_result_reg()
  {
    return memory.read( result_reg );
  }

  template <typename Instruction>
  auto get_unary_func_test()
  {
    const auto test = [this]( const synacor::Word from, const synacor::Word expected ) {
      const auto next_addr = exec<Instruction>( synacor::Word( result_reg ), from );

      BOOST_CHECK_EQUAL( expected, read_result_reg() );
      BOOST_CHECK_EQUAL( next_addr, instruction_addr + synacor::Address( 3 ) );
    };

    return test;
  }

  template <typename Instruction>
  auto get_binary_func_test()
  {
    const auto test = [this]( const synacor::Word from1, const synacor::Word from2, const synacor::Word expected ) {
      const auto next_addr = exec<Instruction>( synacor::Word( result_reg ), from1, from2 );

      BOOST_CHECK_EQUAL( expected, read_result_reg() );
      BOOST_CHECK_EQUAL( next_addr, instruction_addr + synacor::Address( 4 ) );
    };

    return test;
  }

  const synacor::Address instruction_addr = synacor::Address{ 30000 };
  std::stringstream io_ss;

  synacor::MemoryStorage memory;
  synacor::Stack stack;
  synacor::IO io;

  const synacor::Address result_reg      = memory.get_register( 0 );
  const synacor::Address reg_with_42_num = memory.get_register( 1 );
};

#define CHECK_IS_NOT_CHANGED( var )                                                                                                        \
  BOOST_SCOPE_EXIT( var, this_ )                                                                                                           \
  {                                                                                                                                        \
    BOOST_CHECK( var == this_->var );                                                                                                      \
  }                                                                                                                                        \
  BOOST_SCOPE_EXIT_END

#define CHECK_MEMORY_IS_NOT_CHANGED CHECK_IS_NOT_CHANGED( memory )
#define CHECK_STACK_IS_NOT_CHANGED CHECK_IS_NOT_CHANGED( stack )

}  // namespace

using namespace synacor;

BOOST_FIXTURE_TEST_SUITE( synacor_instructions_suite, InstructionsFixture )

// HALT

BOOST_AUTO_TEST_CASE( synacor_instructions_halt )
{
  CHECK_STACK_IS_NOT_CHANGED;
  CHECK_MEMORY_IS_NOT_CHANGED;

  const auto next_addr = exec<synacor::instructions::Halt>();
  BOOST_CHECK_EQUAL( Address( 65535 ), next_addr );
}

// SET

BOOST_AUTO_TEST_CASE( synacor_instructions_set )
{
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = get_unary_func_test<synacor::instructions::Set>();

  test( Word( reg_with_42_num ), 42 );
  test( 100, 100 );
}

// PUSH

BOOST_AUTO_TEST_CASE( synacor_instructions_push )
{
  CHECK_MEMORY_IS_NOT_CHANGED;

  const auto test = [this]( const Word from, const Word expected ) {
    const auto next_addr = exec<synacor::instructions::Push>( from );

    BOOST_REQUIRE( !stack.is_empty() );
    BOOST_CHECK_EQUAL( stack.top(), expected );
    BOOST_CHECK_EQUAL( next_addr, instruction_addr + Address( 2 ) );
  };

  test( Word( reg_with_42_num ), 42 );
  test( 100, 100 );
}

// POP

BOOST_AUTO_TEST_CASE( synacor_instructions_pop )
{
  BOOST_CHECK( stack.is_empty() );
  stack.push( 42 );

  const auto next_addr = exec<synacor::instructions::Pop>( Word( result_reg ) );
  BOOST_CHECK_EQUAL( 42, read_result_reg() );

  BOOST_CHECK( stack.is_empty() );
  BOOST_CHECK_EQUAL( next_addr, instruction_addr + Address( 2 ) );
}

// EQ
BOOST_AUTO_TEST_CASE( synacor_instructions_eq )
{
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = get_binary_func_test<synacor::instructions::Eq>();

  test( Word( reg_with_42_num ), Word( reg_with_42_num ), 1 );
  test( Word( reg_with_42_num ), 100, 0 );
  test( 100, Word( reg_with_42_num ), 0 );
  test( 100, 100, 1 );
}

// GT
BOOST_AUTO_TEST_CASE( synacor_instructions_gt )
{
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = get_binary_func_test<synacor::instructions::Gt>();

  test( Word( reg_with_42_num ), Word( reg_with_42_num ), 0 );
  test( Word( reg_with_42_num ), 10, 1 );
  test( 100, Word( reg_with_42_num ), 1 );
  test( 100, 100, 0 );
}

// JMP
BOOST_AUTO_TEST_CASE( synacor_instructions_jmp )
{
  CHECK_MEMORY_IS_NOT_CHANGED;
  CHECK_STACK_IS_NOT_CHANGED;

  const auto next_addr = exec<synacor::instructions::Jmp>( Word( 1234 ) );
  BOOST_CHECK_EQUAL( next_addr, Address( 1234 ) );
}

// JT
BOOST_AUTO_TEST_CASE( synacor_instructions_jt )
{
  CHECK_MEMORY_IS_NOT_CHANGED;
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = [this]( const Word from, const Address expected ) {
    const auto next_addr = exec<synacor::instructions::Jt>( from, Word( 1234 ) );

    BOOST_CHECK_EQUAL( next_addr, expected );
  };

  test( Word( reg_with_42_num ), Address( 1234 ) );
  test( 0, instruction_addr + Address( 3 ) );
}

// JF
BOOST_AUTO_TEST_CASE( synacor_instructions_jf )
{
  CHECK_MEMORY_IS_NOT_CHANGED;
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = [this]( const Word from, const Address expected ) {
    const auto next_addr = exec<synacor::instructions::Jf>( from, Word( 1234 ) );

    BOOST_CHECK_EQUAL( next_addr, expected );
  };

  test( 0, Address( 1234 ) );
  test( Word( reg_with_42_num ), instruction_addr + Address( 3 ) );
}

// ADD
BOOST_AUTO_TEST_CASE( synacor_instructions_add )
{
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = get_binary_func_test<synacor::instructions::Add>();
  test( Word( reg_with_42_num ), Word( reg_with_42_num ), 84 );
  test( 100, 200, 300 );
  test( 32758, 15, 5 );
}

// MULT
BOOST_AUTO_TEST_CASE( synacor_instructions_mult )
{
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = get_binary_func_test<synacor::instructions::Mult>();
  test( Word( reg_with_42_num ), Word( reg_with_42_num ), 1764 );
  test( 100, 200, 20000 );
  test( 32758, 15, 32618 );
}

// MOD
BOOST_AUTO_TEST_CASE( synacor_instructions_mod )
{
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = get_binary_func_test<synacor::instructions::Mod>();
  test( Word( reg_with_42_num ), Word( reg_with_42_num ), 0 );
  test( 100, 200, 100 );
  test( 32758, 15, 13 );
}

// AND
BOOST_AUTO_TEST_CASE( synacor_instructions_and )
{
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = get_binary_func_test<synacor::instructions::And>();
  test( Word( reg_with_42_num ), Word( reg_with_42_num ), 42 );
  test( 100, 200, 64 );
  test( 32758, 15, 6 );
}

// OR
BOOST_AUTO_TEST_CASE( synacor_instructions_or )
{
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = get_binary_func_test<synacor::instructions::Or>();
  test( Word( reg_with_42_num ), Word( reg_with_42_num ), 42 );
  test( 100, 200, 236 );
  test( 32758, 15, 32767 );
}

// NOT
BOOST_AUTO_TEST_CASE( synacor_instructions_not )
{
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = get_unary_func_test<synacor::instructions::Not>();
  test( Word( reg_with_42_num ), 32725 );
  test( 100, 32667 );
  test( 0, 32767 );
}

// RMEM
BOOST_AUTO_TEST_CASE( synacor_instructions_rmem )
{
  CHECK_STACK_IS_NOT_CHANGED;

  memory.store( Address( 30000 ), 123 );
  BOOST_CHECK_NE( 123, read_result_reg() );
  exec<synacor::instructions::RMem>( Word( result_reg ), Word( 30000 ) );
  BOOST_CHECK_EQUAL( 123, read_result_reg() );

  memory.store( Address( 42 ), 4242 );
  BOOST_CHECK_NE( 4242, read_result_reg() );
  exec<synacor::instructions::RMem>( Word( result_reg ), Word( reg_with_42_num ) );
  BOOST_CHECK_EQUAL( 4242, read_result_reg() );
}

// WMEM
BOOST_AUTO_TEST_CASE( synacor_instructions_wmem )
{
  CHECK_STACK_IS_NOT_CHANGED;

  BOOST_CHECK_NE( 42, memory.read( Address( 30000 ) ) );
  exec<synacor::instructions::WMem>( Word( 30000 ), Word( reg_with_42_num ) );
  BOOST_CHECK_EQUAL( 42, memory.read( Address( 30000 ) ) );

  BOOST_CHECK_NE( 12345, memory.read( Address( 42 ) ) );
  exec<synacor::instructions::WMem>( Word( reg_with_42_num ), Word( 12345 ) );
  BOOST_CHECK_EQUAL( 12345, memory.read( Address( 42 ) ) );
}

// CALL
BOOST_AUTO_TEST_CASE( synacor_instructions_call )
{
  CHECK_MEMORY_IS_NOT_CHANGED;

  BOOST_CHECK( stack.is_empty() );
  BOOST_CHECK( Address( 10000 ) != instruction_addr );
  const auto next_addr = exec<synacor::instructions::Call>( Word( 10000 ) );

  BOOST_CHECK_EQUAL( next_addr, Address( 10000 ) );
  BOOST_CHECK( !stack.is_empty() );
  BOOST_CHECK( instruction_addr + Address( 2 ) == Address( Word( stack.pop() ) ) );
}

// RET
BOOST_AUTO_TEST_CASE( synacor_instructions_ret )
{
  CHECK_MEMORY_IS_NOT_CHANGED;
  CHECK_STACK_IS_NOT_CHANGED;

  exec<synacor::instructions::Call>( Word( 10000 ) );
  const auto next_addr = exec<synacor::instructions::Ret>();

  BOOST_CHECK( instruction_addr + Address( 2 ) == next_addr );
}

// OUT
BOOST_AUTO_TEST_CASE( synacor_instructions_out )
{
  CHECK_MEMORY_IS_NOT_CHANGED;
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = [this]( const Word from, const char expected ) {
    const auto next_addr = exec<synacor::instructions::Out>( from );

    BOOST_CHECK_EQUAL( expected, io_ss.get() );
    BOOST_CHECK_EQUAL( next_addr, instruction_addr + Address( 2 ) );
  };

  test( Word( reg_with_42_num ), '*' );
  test( 100, 'd' );
}

// IN
BOOST_AUTO_TEST_CASE( synacor_instructions_in )
{
  CHECK_STACK_IS_NOT_CHANGED;

  const auto test = [this]( const char expected ) {
    const auto next_addr = exec<synacor::instructions::In>( Word( result_reg ) );

    BOOST_CHECK_EQUAL( expected, read_result_reg() );
    BOOST_CHECK_EQUAL( next_addr, instruction_addr + Address( 2 ) );
  };

  io_ss << "abc";
  test( 'a' );
  test( 'b' );
  test( 'c' );
}

// NOOP

BOOST_AUTO_TEST_CASE( synacor_instructions_noop )
{
  CHECK_MEMORY_IS_NOT_CHANGED;
  CHECK_STACK_IS_NOT_CHANGED;

  const auto next_addr = exec<synacor::instructions::Noop>();
  BOOST_CHECK_EQUAL( next_addr, instruction_addr + Address( 1 ) );
}

BOOST_AUTO_TEST_SUITE_END()
