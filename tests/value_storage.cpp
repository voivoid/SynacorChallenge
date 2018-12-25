#include "boost/test/unit_test.hpp"

#include "synacor/value_storage.h"


BOOST_AUTO_TEST_CASE( synacor_value_tests )
{
  {
    const synacor::Word w = 0;
    BOOST_CHECK( synacor::is_valid( w ) );
    BOOST_CHECK( synacor::is_number( w ) );
  }

  {
    const synacor::Word w  = 42;
    BOOST_CHECK( synacor::is_valid( w ) );
    BOOST_CHECK( synacor::is_number( w ) );
  }

  {
    const synacor::Word w = 32767;
    BOOST_CHECK( synacor::is_valid( w ) );
    BOOST_CHECK( synacor::is_number( w ) );
  }

  {
    const synacor::Word w  = 32768;
    BOOST_CHECK( synacor::is_valid( w ) );
    BOOST_CHECK( synacor::is_register( w ) );
  }

  {
    const synacor::Word w  = 32770;
    BOOST_CHECK( synacor::is_valid( w ) );
    BOOST_CHECK( synacor::is_register( w ) );
  }

  {
    const synacor::Word w  = 32775;
    BOOST_CHECK( synacor::is_valid( w ) );
    BOOST_CHECK( synacor::is_register( w ) );
  }

  {
    const synacor::Word w  = 32776; 
    BOOST_CHECK( !synacor::is_valid( w ) );
  }

  {
    const synacor::Word w = 60000;
    BOOST_CHECK( !synacor::is_valid( w ) );
  }

  {
    const synacor::Word w = 65535;
    BOOST_CHECK( !synacor::is_valid( w ) );
  }
}
