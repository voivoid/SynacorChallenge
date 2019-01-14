#include "synacor/arch.h"

#include <iostream>

namespace synacor
{
inline std::ostream& operator<<( std::ostream& s, const synacor::Address addr )
{
  return s << type_safe::get( addr );
}
}  // namespace synacor
