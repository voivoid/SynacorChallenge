#include "boost/test/unit_test.hpp"

#include "synacor/stack.h"

struct StackFixture
{
  synacor::Stack stack;
};

BOOST_FIXTURE_TEST_CASE( synacor_stack_test, StackFixture )
{
    BOOST_CHECK( stack.is_empty() );
    stack.push( 42 );
    BOOST_CHECK( !stack.is_empty() );
    BOOST_CHECK_EQUAL( 42, stack.top() );
    BOOST_CHECK_EQUAL( 42, stack.pop() );
    BOOST_CHECK( stack.is_empty() );
}
