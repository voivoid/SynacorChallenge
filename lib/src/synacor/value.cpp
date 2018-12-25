#include "synacor/value.h"
#include <limits>

namespace
{
static const auto max_number = std::numeric_limits<synacor::Number>::max();
}

namespace synacor
{

bool is_number( Word word )
{
  return word <= max_number;
}

bool is_register( Word word )
{
  return !is_number( word ) && word <= static_cast<size_t>( max_number ) + RegistersCount;
}

bool is_valid( Word word )
{
  return is_number( word ) || is_register( word );
}

Value interpret_word( Word word )
{
  if ( is_number( word ) )
  {
    return Number( word );
  }
  else if ( is_register( word ) )
  {
    return Register{ static_cast<size_t>( word - max_number - 1 ) };
  }
  else
  {
    return InvalidValue{};
  }
}

}  // namespace synacor
