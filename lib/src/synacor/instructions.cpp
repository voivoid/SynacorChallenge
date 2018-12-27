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
Address Set::execute( MemoryStorage& memory, Stack&, const Address current_address )
{
  SYNACOR_ENSURE( is_register( a ) );
  SYNACOR_ENSURE( is_valid_value( b ) );

  memory.store( Address( a ), Word( get_value( memory, b ) ) );

  return calc_next_instruction_address( current_address );
}

/*
   push: 2 a
   push <a> onto the stack
*/
Address Push::execute( MemoryStorage& memory, Stack& stack, const Address current_address )
{
  SYNACOR_ENSURE( is_valid_value( a ) );
  stack.push( get_value( memory, a ) );

  return calc_next_instruction_address( current_address );
}

/*
   pop: 3 a
   remove the top element from the stack and write it into <a>; empty stack = error
  */
Address Pop::execute( MemoryStorage& memory, Stack& stack, const Address current_address )
{
  SYNACOR_ENSURE( !stack.is_empty() );
  SYNACOR_ENSURE( is_register( a ) );

  memory.store( Address( a ), Word( stack.pop() ) );

  return calc_next_instruction_address( current_address );
}

/*
   add: 9 a b c
   assign into <a> the sum of <b> and <c> (modulo 32768)
*/
Address Add::execute( MemoryStorage& memory, Stack&, const Address current_address )
{
  SYNACOR_ENSURE( is_register( a ) );
  SYNACOR_ENSURE( is_valid_value( b ) );
  SYNACOR_ENSURE( is_valid_value( c ) );

  memory.store( Address( a ), Word( get_value( memory, b ) + get_value( memory, c ) ) );

  return calc_next_instruction_address( current_address );
}

/*
   noop: 21
   no operation
*/
Address Noop::execute( MemoryStorage&, Stack&, const Address current_address )
{
  return calc_next_instruction_address( current_address );
}



}  // namespace instructions

}  // namespace synacor
