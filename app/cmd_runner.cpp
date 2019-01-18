#include "cmd_runner.h"

#include "synacor/synacor.h"

#include <cstddef>
#include <iostream>

#include "boost/hof/lift.hpp"
#include "boost/iostreams/device/mapped_file.hpp"

namespace
{
boost::iostreams::mapped_file_source open_file( const std::string& filepath )
{
  boost::iostreams::mapped_file_source file;

  file.open( filepath );
  if ( !file.is_open() )
  {
    throw;
  }

  return file;
}

void run_binary( const std::string& filepath )
{
  auto mapped_file = open_file( filepath );

  const std::byte* const data = reinterpret_cast<const std::byte*>( mapped_file.data() );
  synacor::execute( data, mapped_file.size(), std::cin, std::cout );
}

void exec_cmd( const synacor::RunBinary& cmd )
{
  run_binary( cmd.filepath );
}

void exec_cmd( const synacor::ShowHelp& )
{
}

void exec_cmd( const synacor::UnknownCmd& )
{
}

}  // namespace

namespace synacor
{
void run_cmd( const Cmd& cmd )
{
  std::visit( BOOST_HOF_LIFT( exec_cmd ), cmd );
}
}  // namespace synacor
