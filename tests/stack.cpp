#include "boost/test/unit_test.hpp"

#include "synacor/assert.h"
#include "synacor/stack.h"

struct StackFixture
{
  synacor::Stack stack;
};

BOOST_FIXTURE_TEST_CASE( synacor_stack_test, StackFixture )
{
  BOOST_CHECK( stack.is_empty() );
  BOOST_CHECK_THROW( stack.pop(), synacor::assertion );

  stack.push( 42 );
  BOOST_REQUIRE( !stack.is_empty() );
  BOOST_CHECK_EQUAL( 42, stack.top() );
  BOOST_CHECK_EQUAL( 42, stack.pop() );
  BOOST_CHECK( stack.is_empty() );

  stack.push( 1 );
  stack.push( 2 );
  stack.push( 3 );
  BOOST_CHECK_EQUAL( 3, stack.top() );
  BOOST_CHECK_EQUAL( 3, stack.pop() );
  BOOST_CHECK_EQUAL( 2, stack.pop() );
  BOOST_CHECK_EQUAL( 1, stack.pop() );
  BOOST_CHECK_THROW( stack.pop(), synacor::assertion );
}
