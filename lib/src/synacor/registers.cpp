#include "synacor/registers.h"
#include "synacor/assert.h"


namespace synacor
{

Registers::Registers() : registers{}
{
}

Number Registers::get_register_value( const Register reg )
{
  return get_register( reg );
}

void Registers::set_register_value( const Register reg, const Number value )
{
  get_register( reg ) = value;
}

Number& Registers::get_register( const Register reg )
{
  SYNACOR_ASSERT( reg.index < RegistersCount );
  return registers[ reg.index ];
}

}  // namespace synacor
