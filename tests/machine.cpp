#include "boost/test/unit_test.hpp"

#include "synacor/machine.h"
#include "synacor/memory_storage.h"
#include "synacor/stack.h"
#include "test_utils.h"

#include <array>
#include <sstream>


struct MachineFixture
{
  MachineFixture() :
      machine{ std::make_unique<synacor::MemoryStorage>(), std::make_unique<synacor::Stack>(), synacor::Address( 0 ), io_ss, io_ss }
  {
  }

  void write_to_mem( const std::initializer_list<synacor::Word>& words, synacor::Address write_addr = synacor::Address{ 0 } )
  {
    for ( const auto word : words )
    {
      machine.memory->store( write_addr++, word );
    }
  }

  void execute()
  {
    synacor::run( machine );
  }

  synacor::Machine machine;
  std::stringstream io_ss;
};

BOOST_FIXTURE_TEST_SUITE( synacor_machine_suite, MachineFixture )

BOOST_AUTO_TEST_CASE( synacor_machine_simple_halt_test )
{
  write_to_mem( { 0 } );
  execute();
  BOOST_CHECK_EQUAL( synacor::Address{ 65535 }, machine.current_address );
}

BOOST_AUTO_TEST_CASE( synacor_machine_arch_spec_test )
{
  write_to_mem( { 9, 32768, 32769, 4, 19, 32768, 0 }, synacor::Address{ 0 } );
  execute();
  BOOST_CHECK_EQUAL( synacor::Address{ 65535 }, machine.current_address );

  BOOST_CHECK_EQUAL( std::string( "\x04" ), io_ss.str() );
}

BOOST_AUTO_TEST_SUITE_END()