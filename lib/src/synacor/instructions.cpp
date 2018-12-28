#include "synacor/instructions.h"

#include "synacor/assert.h"
#include "synacor/memory_storage.h"
#include "synacor/stack.h"

#include <iostream>

namespace
{
bool is_valid_char( const synacor::Number n )
{
  return n >= 0 && n <= 255;
}
}  // namespace

namespace synacor
{

namespace instructions
{

std::ostream* OutputStream = nullptr;
std::istream* InputStream  = nullptr;

void set_ostream( std::ostream* ostream )
{
  OutputStream = ostream;
}

void set_istream( std::istream* istream )
{
  InputStream = istream;
}

/*
   halt: 0
   stop execution and terminate the program
*/
Address Halt::execute(MemoryStorage &, Stack &, Address )
{
    return Address( 0 );
}

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
   eq: 4 a b c
   set <a> to 1 if <b> is equal to <c>; set it to 0 otherwise
*/
Address Eq::execute( MemoryStorage& memory, Stack&, Address current_address )
{
  SYNACOR_ENSURE( is_register( a ) );
  SYNACOR_ENSURE( is_valid_value( b ) );
  SYNACOR_ENSURE( is_valid_value( c ) );

  const bool are_equal = get_value( memory, b ) == get_value( memory, c );
  memory.store( Address( a ), are_equal ? Word( 1 ) : Word( 0 ) );

  return calc_next_instruction_address( current_address );
}

/*
   gt: 5 a b c
   set <a> to 1 if <b> is greater than <c>; set it to 0 otherwise
*/
Address Gt::execute( MemoryStorage& memory, Stack&, Address current_address )
{
  SYNACOR_ENSURE( is_register( a ) );
  SYNACOR_ENSURE( is_valid_value( b ) );
  SYNACOR_ENSURE( is_valid_value( c ) );

  const bool are_greater = get_value( memory, b ) > get_value( memory, c );
  memory.store( Address( a ), are_greater ? 1 : 0 );

  return calc_next_instruction_address( current_address );
}

/*
   jmp: 6 a
   jump to <a>
*/
Address Jmp::execute(MemoryStorage& memory, Stack &, Address )
{
    SYNACOR_ENSURE( is_valid_value( a ) );

    return Address( Word( get_value( memory, a ) ) );
}

/*
   jt: 7 a b
   if <a> is nonzero, jump to <b>
*/
Address Jt::execute(MemoryStorage& memory, Stack &, Address current_address )
{
    SYNACOR_ENSURE( is_valid_value( a ) );
    SYNACOR_ENSURE( is_valid_value( b ) );

    const bool is_a_nonzero = get_value( memory, a ) != 0;
    const Address b_addr = Address( Word( get_value( memory, b ) ) );

    return is_a_nonzero ? b_addr : calc_next_instruction_address( current_address );
}

/*
   jf: 8 a b
   if <a> is zero, jump to <b>
*/
Address Jf::execute(MemoryStorage& memory, Stack &, Address current_address )
{
    SYNACOR_ENSURE( is_valid_value( a ) );
    SYNACOR_ENSURE( is_valid_value( b ) );

    const bool is_a_zero = get_value( memory, a ) == 0;
    const Address b_addr = Address( Word( get_value( memory, b ) ) );

    return is_a_zero ? b_addr : calc_next_instruction_address( current_address );
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
   out: 19 a
   write the character represented by ascii code <a> to the terminal
*/
Address Out::execute( MemoryStorage& memory, Stack&, Address current_address )
{
  SYNACOR_ENSURE( is_valid_value( a ) );

  const Number chr = get_value( memory, a );
  SYNACOR_ENSURE( is_valid_char( chr ) );

  if ( OutputStream )
  {
    OutputStream->put( static_cast<char>( chr ) );
  }

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
