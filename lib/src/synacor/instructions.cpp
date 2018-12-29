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

synacor::Number add_op( const synacor::Number b, const synacor::Number c )
{
  return b + c;
}

synacor::Number mult_op( const synacor::Number b, const synacor::Number c )
{
  return b * c;
}

synacor::Number mod_op( const synacor::Number b, const synacor::Number c )
{
  return b % c;
}

synacor::Number and_op( const synacor::Number b, const synacor::Number c )
{
  return b & c;
}

synacor::Number or_op( const synacor::Number b, const synacor::Number c )
{
  return b | c;
}

template <synacor::Number ( *op )( synacor::Number, synacor::Number )>
void exec_arith_op( synacor::MemoryStorage& memory, const synacor::Word a, const synacor::Word b, const synacor::Word c )
{
  SYNACOR_ENSURE( synacor::is_register( a ) );

  memory.store( synacor::Address( a ), synacor::Word( op( get_value( memory, b ), get_value( memory, c ) ) ) );
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
Address Halt::execute( MemoryStorage&, Stack&, Address )
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

  memory.store( Address( a ), Word( get_value( memory, b ) ) );

  return calc_next_instruction_address( current_address );
}

/*
   push: 2 a
   push <a> onto the stack
*/
Address Push::execute( MemoryStorage& memory, Stack& stack, const Address current_address )
{
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

  const bool are_greater = get_value( memory, b ) > get_value( memory, c );
  memory.store( Address( a ), are_greater ? 1 : 0 );

  return calc_next_instruction_address( current_address );
}

/*
   jmp: 6 a
   jump to <a>
*/
Address Jmp::execute( MemoryStorage& memory, Stack&, Address )
{
  return Address( Word( get_value( memory, a ) ) );
}

/*
   jt: 7 a b
   if <a> is nonzero, jump to <b>
*/
Address Jt::execute( MemoryStorage& memory, Stack&, Address current_address )
{
  const bool is_a_nonzero = get_value( memory, a ) != 0;
  const Address b_addr    = Address( Word( get_value( memory, b ) ) );

  return is_a_nonzero ? b_addr : calc_next_instruction_address( current_address );
}

/*
   jf: 8 a b
   if <a> is zero, jump to <b>
*/
Address Jf::execute( MemoryStorage& memory, Stack&, Address current_address )
{
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
  exec_arith_op<&add_op>( memory, a, b, c );
  return calc_next_instruction_address( current_address );
}

/*
   mult: 10 a b c
   store into <a> the product of <b> and <c> (modulo 32768)
*/

Address Mult::execute( MemoryStorage& memory, Stack&, const Address current_address )
{
  exec_arith_op<&mult_op>( memory, a, b, c );
  return calc_next_instruction_address( current_address );
}

/*
   mod: 11 a b c
   store into <a> the remainder of <b> divided by <c>
*/
Address Mod::execute( MemoryStorage& memory, Stack&, const Address current_address )
{
  exec_arith_op<&mod_op>( memory, a, b, c );
  return calc_next_instruction_address( current_address );
}

/*
   and: 12 a b c
   stores into <a> the bitwise and of <b> and <c>
*/
Address And::execute( MemoryStorage& memory, Stack&, const Address current_address )
{
  exec_arith_op<&and_op>( memory, a, b, c );
  return calc_next_instruction_address( current_address );
}

/*
   or: 13 a b c
   stores into <a> the bitwise or of <b> and <c>
*/
Address Or::execute( MemoryStorage& memory, Stack&, const Address current_address )
{
  exec_arith_op<&or_op>( memory, a, b, c );
  return calc_next_instruction_address( current_address );
}

/*
   not: 14 a b
   stores 15-bit bitwise inverse of <b> in <a>
*/
Address Not::execute( MemoryStorage& memory, Stack&, Address current_address )
{
  SYNACOR_ENSURE( is_register( a ) );

  memory.store( Address( a ), Word( ~get_value( memory, b ) ) );

  return calc_next_instruction_address( current_address );
}

/*
   rmem: 15 a b
   read memory at address <b> and write it to <a>
*/
Address RMem::execute( MemoryStorage& memory, Stack&, Address current_address )
{
  SYNACOR_ENSURE( is_register( a ) );
  SYNACOR_ENSURE( is_valid_address( Address( b ) ) );

  memory.store( Address( a ), memory.read( Address( b ) ) );

  return calc_next_instruction_address( current_address );
}

/*
   wmem: 16 a b
   write the value from <b> into memory at address <a>
*/
Address WMem::execute( MemoryStorage& memory, Stack&, Address current_address )
{
  SYNACOR_ENSURE( is_valid_address( Address( a ) ) );

  memory.store( Address( a ), Word( get_value( memory, b ) ) );

  return calc_next_instruction_address( current_address );
}


/*
   out: 19 a
   write the character represented by ascii code <a> to the terminal
*/
Address Out::execute( MemoryStorage& memory, Stack&, Address current_address )
{
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
