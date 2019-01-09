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

template <typename ArithmFunc>
void exec_arith_op( synacor::MemoryStorage& memory, const synacor::Word a, const synacor::Word b, const synacor::Word c )
{
  SYNACOR_ENSURE( synacor::is_register( a ) );

  memory.store( synacor::Address( a ), synacor::Word( ArithmFunc{}( get_value( memory, b ), get_value( memory, c ) ) ) );
}

}  // namespace

namespace synacor
{

namespace instructions
{

/*
   halt: 0
   stop execution and terminate the program
*/
Address Halt::execute( Environment& )
{
  return Address( 0 );
}

/*
   set: 1 a b
   set register <a> to the value of <b>
*/
Address Set::execute( Environment& env )
{
  SYNACOR_ENSURE( is_register( a ) );

  env.memory.store( Address( a ), Word( get_value( env.memory, b ) ) );

  return calc_next_instruction_address( env.current_address );
}

/*
   push: 2 a
   push <a> onto the stack
*/
Address Push::execute( Environment& env )
{
  env.stack.push( get_value( env.memory, a ) );

  return calc_next_instruction_address( env.current_address );
}

/*
   pop: 3 a
   remove the top element from the stack and write it into <a>; empty stack = error
*/
Address Pop::execute( Environment& env )
{
  SYNACOR_ENSURE( !env.stack.is_empty() );
  SYNACOR_ENSURE( is_register( a ) );

  env.memory.store( Address( a ), Word( env.stack.pop() ) );

  return calc_next_instruction_address( env.current_address );
}

/*
   eq: 4 a b c
   set <a> to 1 if <b> is equal to <c>; set it to 0 otherwise
*/
Address Eq::execute( Environment& env )
{
  SYNACOR_ENSURE( is_register( a ) );

  const bool are_equal = get_value( env.memory, b ) == get_value( env.memory, c );
  env.memory.store( Address( a ), are_equal ? Word( 1 ) : Word( 0 ) );

  return calc_next_instruction_address( env.current_address );
}

/*
   gt: 5 a b c
   set <a> to 1 if <b> is greater than <c>; set it to 0 otherwise
*/
Address Gt::execute( Environment& env )
{
  SYNACOR_ENSURE( is_register( a ) );

  const bool are_greater = get_value( env.memory, b ) > get_value( env.memory, c );
  env.memory.store( Address( a ), are_greater ? 1 : 0 );

  return calc_next_instruction_address( env.current_address );
}

/*
   jmp: 6 a
   jump to <a>
*/
Address Jmp::execute( Environment& env )
{
  return Address( Word( get_value( env.memory, a ) ) );
}

/*
   jt: 7 a b
   if <a> is nonzero, jump to <b>
*/
Address Jt::execute( Environment& env )
{
  const bool is_a_nonzero = get_value( env.memory, a ) != 0;
  const Address b_addr    = Address( Word( get_value( env.memory, b ) ) );

  return is_a_nonzero ? b_addr : calc_next_instruction_address( env.current_address );
}

/*
   jf: 8 a b
   if <a> is zero, jump to <b>
*/
Address Jf::execute( Environment& env )
{
  const bool is_a_zero = get_value( env.memory, a ) == 0;
  const Address b_addr = Address( Word( get_value( env.memory, b ) ) );

  return is_a_zero ? b_addr : calc_next_instruction_address( env.current_address );
}

/*
   add: 9 a b c
   assign into <a> the sum of <b> and <c> (modulo 32768)
*/
Address Add::execute( Environment& env )
{
  exec_arith_op<std::plus<Number>>( env.memory, a, b, c );
  return calc_next_instruction_address( env.current_address );
}

/*
   mult: 10 a b c
   store into <a> the product of <b> and <c> (modulo 32768)
*/

Address Mult::execute( Environment& env )
{
  exec_arith_op<std::multiplies<Number>>( env.memory, a, b, c );
  return calc_next_instruction_address( env.current_address );
}

/*
   mod: 11 a b c
   store into <a> the remainder of <b> divided by <c>
*/
Address Mod::execute( Environment& env )
{
  exec_arith_op<std::modulus<Number>>( env.memory, a, b, c );
  return calc_next_instruction_address( env.current_address );
}

/*
   and: 12 a b c
   stores into <a> the bitwise and of <b> and <c>
*/
Address And::execute( Environment& env )
{
  exec_arith_op<std::bit_and<Number>>( env.memory, a, b, c );
  return calc_next_instruction_address( env.current_address );
}

/*
   or: 13 a b c
   stores into <a> the bitwise or of <b> and <c>
*/
Address Or::execute( Environment& env )
{
  exec_arith_op<std::bit_or<Number>>( env.memory, a, b, c );
  return calc_next_instruction_address( env.current_address );
}

/*
   not: 14 a b
   stores 15-bit bitwise inverse of <b> in <a>
*/
Address Not::execute( Environment& env )
{
  SYNACOR_ENSURE( is_register( a ) );

  env.memory.store( Address( a ), Word( ~get_value( env.memory, b ) ) );

  return calc_next_instruction_address( env.current_address );
}

/*
   rmem: 15 a b
   read memory at address <b> and write it to <a>
*/
Address RMem::execute( Environment& env )
{
  SYNACOR_ENSURE( is_register( a ) );
  SYNACOR_ENSURE( is_valid_address( Address( b ) ) );

  env.memory.store( Address( a ), env.memory.read( Address( b ) ) );

  return calc_next_instruction_address( env.current_address );
}

/*
   wmem: 16 a b
   write the value from <b> into memory at address <a>
*/
Address WMem::execute( Environment& env )
{
  SYNACOR_ENSURE( is_valid_address( Address( a ) ) );

  env.memory.store( Address( a ), Word( get_value( env.memory, b ) ) );

  return calc_next_instruction_address( env.current_address );
}

/*
call: 17 a
  write the address of the next instruction to the stack and jump to <a>
*/
Address Call::execute( Environment& env )
{
  const Address next_instruction = calc_next_instruction_address( env.current_address );
  env.stack.push( Word( next_instruction ) );

  return Address( Word( get_value( env.memory, a ) ) );
}

/*
ret: 18
  remove the top element from the stack and jump to it; empty stack = halt
*/
Address Ret::execute( Environment& env )
{
  if ( env.stack.is_empty() )
  {
    Halt{}.execute( env );
    return calc_next_instruction_address( env.current_address );
  }

  return Address( Word( env.stack.pop() ) );
}

/*
   out: 19 a
   write the character represented by ascii code <a> to the terminal
*/
Address Out::execute( Environment& env )
{
  const Number chr = get_value( env.memory, a );
  SYNACOR_ENSURE( is_valid_char( chr ) );

  env.iostream.put( static_cast<char>( chr ) );

  return calc_next_instruction_address( env.current_address );
}

/*
   in: 20 a
   read a character from the terminal and write its ascii code to <a>;
   it can be assumed that once input starts, it will continue until a newline is encountered;
   this means that you can safely read whole lines from the keyboard and trust that they will be fully read
*/
Address In::execute( Environment& env )
{
  SYNACOR_ENSURE( is_register( a ) );

  const auto chr = env.iostream.get();
  SYNACOR_ENSURE( is_valid_char( chr ) );
  env.memory.store( Address( a ), Word( chr ) );

  return calc_next_instruction_address( env.current_address );
}

/*
   noop: 21
   no operation
*/
Address Noop::execute( Environment& env )
{
  return calc_next_instruction_address( env.current_address );
}



}  // namespace instructions

}  // namespace synacor
