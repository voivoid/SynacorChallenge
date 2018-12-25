#include "synacor/value.h"
#include <limits>

namespace synacor
{

Value interpret_word( Word word )
{
  static const auto max_number = std::numeric_limits<Number>::max();
  if ( word <= max_number )
  {
    return Number( word );
  }
  else if ( word <= static_cast<size_t>( max_number ) + RegistersCount )
  {
    return Register{ static_cast<size_t>( word - max_number - 1 ) };
  }
  else
  {
    return InvalidValue{};
  }
}

}  // namespace synacor
