#include "synacor/instructions.h"

#include "synacor/assert.h"
#include "synacor/memory_storage.h"
#include "synacor/stack.h"

namespace synacor
{

namespace instructions
{

/*
   set: 1 a b
   set register <a> to the value of <b>
*/
void Set::execute( MemoryStorage& memory, Stack& )
{
  SYNACOR_ENSURE( is_register( a ) );
  SYNACOR_ENSURE( is_valid_value( b ) );

  memory.store( Address( a ), Word( get_value( memory, b ) ) );
}

/*
   add: 9 a b c
   assign into <a> the sum of <b> and <c> (modulo 32768)
*/
void Add::execute( MemoryStorage& memory, Stack& )
{
  SYNACOR_ENSURE( is_register( a ) );
  SYNACOR_ENSURE( is_valid_value( b ) );
  SYNACOR_ENSURE( is_valid_value( c ) );

  memory.store( Address( a ), Word( get_value( memory, b ) + get_value( memory, c ) ) );
}

/*
   noop: 21
   no operation
*/
void Noop::execute( MemoryStorage&, Stack& )
{
}



}  // namespace instructions

}  // namespace synacor
