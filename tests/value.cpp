#include "boost/test/unit_test.hpp"

#include "synacor/value.h"
#include <ostream>

namespace synacor
{

bool operator==( const Register r1, const Register r2 )
{
  return r1.index == r2.index;
}

std::ostream& operator<<( std::ostream& ostream, const Register r )
{
  return ostream << r.index;
}

}  // namespace synacor

BOOST_AUTO_TEST_CASE( synacor_value_tests )
{
  {
    const auto w = synacor::interpret_word( 0 );
    BOOST_REQUIRE( std::holds_alternative<synacor::Number>( w ) );
    BOOST_CHECK_EQUAL( 0, std::get<synacor::Number>( w ) );
  }

  {
    const auto w = synacor::interpret_word( 42 );
    BOOST_REQUIRE( std::holds_alternative<synacor::Number>( w ) );
    BOOST_CHECK_EQUAL( 42, std::get<synacor::Number>( w ) );
  }

  {
    const auto w = synacor::interpret_word( 32767 );
    BOOST_REQUIRE( std::holds_alternative<synacor::Number>( w ) );
    BOOST_CHECK_EQUAL( 32767, std::get<synacor::Number>( w ) );
  }

  {
    const auto w = synacor::interpret_word( 32768 );
    BOOST_REQUIRE( std::holds_alternative<synacor::Register>( w ) );
    BOOST_CHECK_EQUAL( synacor::Register{ 0 }, std::get<synacor::Register>( w ) );
  }

  {
    const auto w = synacor::interpret_word( 32770 );
    BOOST_REQUIRE( std::holds_alternative<synacor::Register>( w ) );
    BOOST_CHECK_EQUAL( synacor::Register{ 2 }, std::get<synacor::Register>( w ) );
  }

  {
    const auto w = synacor::interpret_word( 32775 );
    BOOST_REQUIRE( std::holds_alternative<synacor::Register>( w ) );
    BOOST_CHECK_EQUAL( synacor::Register{ 7 }, std::get<synacor::Register>( w ) );
  }

  {
    const auto w = synacor::interpret_word( 32776 );
    BOOST_CHECK( std::holds_alternative<synacor::InvalidValue>( w ) );
  }

  {
    const auto w = synacor::interpret_word( 60000 );
    BOOST_CHECK( std::holds_alternative<synacor::InvalidValue>( w ) );
  }

  {
    const auto w = synacor::interpret_word( 65535 );
    BOOST_CHECK( std::holds_alternative<synacor::InvalidValue>( w ) );
  }
}
