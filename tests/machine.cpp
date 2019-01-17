#include "boost/test/unit_test.hpp"

#include "synacor/machine.h"
#include "synacor/memory_storage.h"
#include "synacor/stack.h"
#include "synacor/io.h"
#include "test_utils.h"

#include <array>
#include <sstream>


struct MachineFixture
{
  MachineFixture() :
      io{ io_ss, io_ss },
      machine{ memory, stack, io }
  {
  }

  void write_to_mem( const std::initializer_list<synacor::Word>& words, synacor::Address write_addr = synacor::Address{ 0 } )
  {
    for ( const auto word : words )
    {
      machine.memory.store( write_addr++, word );
    }
  }

  void execute()
  {
    synacor::run( machine );
  }

  std::stringstream io_ss;

  synacor::MemoryStorage memory;
  synacor::Stack stack;
  synacor::IO io;

  synacor::Machine machine;

};

BOOST_FIXTURE_TEST_SUITE( synacor_machine_suite, MachineFixture )

BOOST_AUTO_TEST_CASE( synacor_machine_simple_halt_test )
{
  write_to_mem( { 0 } );
  execute();
}

BOOST_AUTO_TEST_CASE( synacor_machine_arch_spec_test )
{
  write_to_mem( { 9, 32768, 32769, 4, 19, 32768, 0 }, synacor::Address{ 0 } );
  execute();

  BOOST_CHECK_EQUAL( std::string( "\x04" ), io_ss.str() );
}

BOOST_AUTO_TEST_SUITE_END()
