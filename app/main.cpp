#include "synacor/synacor.h"
#include <boost/iostreams/device/mapped_file.hpp>

#include <cstddef>

int main()
{
  boost::iostreams::mapped_file_source file;
  file.open( "challenge.bin" );
  if ( !file.is_open() )
  {
    throw;
  }

  const auto* data = reinterpret_cast<const std::byte*>( file.data() );
  synacor::execute( data, file.size() );
}
