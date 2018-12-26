#include "boost/test/unit_test.hpp"

#include "synacor/memory_storage.h"

struct MemoryStorageFixture
{
  synacor::MemoryStorage memory;
};

BOOST_AUTO_TEST_CASE( synacor_value_tests )
{
  {
    const synacor::Word w = 0;
    BOOST_CHECK( synacor::is_valid_address( synacor::Address( w ) ) );
    BOOST_CHECK( synacor::is_number( w ) );
  }

  {
    const synacor::Word w = 42;
    BOOST_CHECK( synacor::is_valid_address( synacor::Address( w ) ) );
    BOOST_CHECK( synacor::is_number( w ) );
  }

  {
    const synacor::Word w = 32767;
    BOOST_CHECK( synacor::is_valid_address( synacor::Address( w ) ) );
    BOOST_CHECK( synacor::is_number( w ) );
  }

  {
    const synacor::Word w = 32768;
    BOOST_CHECK( synacor::is_valid_address( synacor::Address( w ) ) );
    BOOST_CHECK( synacor::is_register( w ) );
  }

  {
    const synacor::Word w = 32770;
    BOOST_CHECK( synacor::is_valid_address( synacor::Address( w ) ) );
    BOOST_CHECK( synacor::is_register( w ) );
  }

  {
    const synacor::Word w = 32775;
    BOOST_CHECK( synacor::is_valid_address( synacor::Address( w ) ) );
    BOOST_CHECK( synacor::is_register( w ) );
  }

  {
    const synacor::Word w = 32776;
    BOOST_CHECK( !synacor::is_valid_address( synacor::Address( w ) ) );
  }

  {
    const synacor::Word w = 60000;
    BOOST_CHECK( !synacor::is_valid_address( synacor::Address( w ) ) );
  }

  {
    const synacor::Word w = 65535;
    BOOST_CHECK( !synacor::is_valid_address( synacor::Address( w ) ) );
  }
}

BOOST_FIXTURE_TEST_CASE( synacor_memory_storage_check_memory_zero_initialized, MemoryStorageFixture )
{
}

BOOST_FIXTURE_TEST_CASE( synacor_memory_storage_check_read_writes, MemoryStorageFixture )
{
  BOOST_CHECK_EQUAL( 0, memory.read( synacor::Address{ 0 } ) );
  memory.store( synacor::Address{ 0 }, 42 );
  BOOST_CHECK_EQUAL( 42, memory.read( synacor::Address{ 0 } ) );
}
