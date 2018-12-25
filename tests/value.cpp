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
    const synacor::Word w = 0;
    const synacor::Value v = synacor::interpret_word( w );
    BOOST_CHECK( synacor::is_valid( w ) );
    BOOST_CHECK( synacor::is_number( w ) );
    BOOST_REQUIRE( std::holds_alternative<synacor::Number>( v ) );
    BOOST_CHECK_EQUAL( 0, std::get<synacor::Number>( v ) );
  }

  {
    const synacor::Word w  = 42;
    const synacor::Value v = synacor::interpret_word( w );
    BOOST_CHECK( synacor::is_valid( w ) );
    BOOST_CHECK( synacor::is_number( w ) );
    BOOST_REQUIRE( std::holds_alternative<synacor::Number>( v ) );
    BOOST_CHECK_EQUAL( 42, std::get<synacor::Number>( v ) );
  }

  {
    const synacor::Word w = 32767;
    const synacor::Value v = synacor::interpret_word( w );
    BOOST_CHECK( synacor::is_valid( w ) );
    BOOST_CHECK( synacor::is_number( w ) );
    BOOST_REQUIRE( std::holds_alternative<synacor::Number>( v ) );
    BOOST_CHECK_EQUAL( 32767, std::get<synacor::Number>( v ) );
  }

  {
    const synacor::Word w  = 32768;
    const synacor::Value v = synacor::interpret_word( w );
    BOOST_CHECK( synacor::is_valid( w ) );
    BOOST_CHECK( synacor::is_register( w ) );
    BOOST_REQUIRE( std::holds_alternative<synacor::Register>( v ) );
    BOOST_CHECK_EQUAL( synacor::Register{ 0 }, std::get<synacor::Register>( v ) );
  }

  {
    const synacor::Word w  = 32770;
    const synacor::Value v = synacor::interpret_word( w );
    BOOST_CHECK( synacor::is_valid( w ) );
    BOOST_CHECK( synacor::is_register( w ) );
    BOOST_REQUIRE( std::holds_alternative<synacor::Register>( v ) );
    BOOST_CHECK_EQUAL( synacor::Register{ 2 }, std::get<synacor::Register>( v ) );
  }

  {
    const synacor::Word w  = 32775;
    const synacor::Value v = synacor::interpret_word( w );
    BOOST_CHECK( synacor::is_valid( w ) );
    BOOST_CHECK( synacor::is_register( w ) );
    BOOST_REQUIRE( std::holds_alternative<synacor::Register>( v ) );
    BOOST_CHECK_EQUAL( synacor::Register{ 7 }, std::get<synacor::Register>( v ) );
  }

  {
    const synacor::Word w  = 32776; 
    const synacor::Value v = synacor::interpret_word( w );
    BOOST_CHECK( !synacor::is_valid( w ) );
    BOOST_CHECK( std::holds_alternative<synacor::InvalidValue>( v ) );
  }

  {
    const synacor::Word w = 60000;
    const auto v = synacor::interpret_word( w );
    BOOST_CHECK( !synacor::is_valid( w ) );
    BOOST_CHECK( std::holds_alternative<synacor::InvalidValue>( v ) );
  }

  {
    const synacor::Word w = 65535;
    const auto v = synacor::interpret_word( w );
    BOOST_CHECK( !synacor::is_valid( w ) );
    BOOST_CHECK( std::holds_alternative<synacor::InvalidValue>( v ) );
  }
}
