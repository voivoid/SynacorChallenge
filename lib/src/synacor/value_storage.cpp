#include "synacor/value_storage.h"

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
}