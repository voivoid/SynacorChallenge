#include "synacor/io.h"

#include <iostream>
#include <stdexcept>

namespace synacor
{
IO::IO( std::istream& input, std::ostream& output ) : in( input ), out( output )
{
}

void IO::put( char c )
{
  out.put( c );
}

char IO::get()
{
  const auto input = in.get();
  if ( !isprint( input ) && !iscntrl( input ) )
  {
    throw std::runtime_error( "unexpected input char" );
  }
  return static_cast<char>( input );
}
}  // namespace synacor
