#include "cmd_parser.h"

#include "boost/program_options.hpp"

namespace synacor
{
Cmd parse_cmd_line( int argc, char* argv[] )
{
  boost::program_options::options_description description( "Options" );
  description.add_options()( "help", "show help message" )(
      "binary", boost::program_options::value<std::string>(), "filepath to a binary to execute" );

  boost::program_options::positional_options_description po_description;
  po_description.add( "binary", -1 );

  boost::program_options::variables_map vars_map;
  boost::program_options::store(
      boost::program_options::command_line_parser( argc, argv ).options( description ).positional( po_description ).run(), vars_map );
  boost::program_options::notify( vars_map );

  if ( vars_map.count( "binary" ) )
  {
    return RunBinaryCmd{ vars_map[ "binary" ].as<std::string>() };
  }

  std::ostringstream help_ss;
  description.print( help_ss );
  return ShowHelpCmd{ help_ss.str() };
}
}  // namespace synacor
