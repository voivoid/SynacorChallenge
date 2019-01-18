#include "cmd_parser.h"
#include "cmd_runner.h"

#include <iostream>
#include <stdexcept>

namespace
{

void safe_main( int argc, char* argv[] )
{
  const auto cmd = synacor::parse_cmd_line( argc, argv );
  synacor::run_cmd( cmd );
}
}  // namespace

int main( int argc, char* argv[] )
{
  try
  {
    safe_main( argc, argv );
    return 0;
  }
  catch ( const std::exception& ex )
  {
    std::cerr << "Unhandled exception: " << ex.what() << std::endl;
  }
  catch ( ... )
  {
    std::cerr << "Unhandled unknown exception" << std::endl;
  }

  return 1;
}
