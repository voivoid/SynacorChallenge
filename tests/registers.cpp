#include "boost/test/unit_test.hpp"

#include "synacor/registers.h"

#include <functional>

namespace
{

struct RegisterFixture
{
  RegisterFixture() : registers( registers_data ){
  }

  void for_all_registers( std::function<void( synacor::Register )> f )
  {
    for ( size_t i = 0; i < synacor::RegistersCount; ++i )
    {
      f( synacor::Register{ i } );
    }
  }

  synacor::Number registers_data[synacor::RegistersCount];
  synacor::Registers registers;
};

}  // namespace

BOOST_FIXTURE_TEST_CASE( synacor_registers_should_be_zeroed_on_construction, RegisterFixture )
{
  for_all_registers( [this]( const synacor::Register r ) { BOOST_CHECK_EQUAL( 0, registers.get_register_value( r ) ); } );
}

BOOST_FIXTURE_TEST_CASE( synacor_registers_should_store_new_values, RegisterFixture )
{
  for_all_registers( [this]( const synacor::Register r ) {
    const synacor::Number n = r.index + 1;

    BOOST_CHECK_NE( n, registers.get_register_value( r ) );
    registers.set_register_value( r, n );
    BOOST_CHECK_EQUAL( n, registers.get_register_value( r ) );
  } );
}
