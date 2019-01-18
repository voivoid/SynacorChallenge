#include "synacor/arch.h"

namespace synacor
{

bool is_number( const Word word )
{
  return word <= NumberMax;
}

bool is_register( const Word word )
{
  return !is_number( word ) && word <= NumberMax + RegistersCount;
}

bool is_valid_address( const Address address )
{
  return is_number( Word( address ) ) || is_register( Word( address ) );
}

}  // namespace synacor
