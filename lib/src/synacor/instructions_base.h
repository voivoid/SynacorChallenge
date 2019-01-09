#pragma once

#include "synacor/arch.h"

#include <cstddef>

#include <iosfwd>

namespace synacor
{

class MemoryStorage;
class Stack;

struct Environment
{
  MemoryStorage& memory;
  Stack& stack;
  Address current_address;
  std::iostream& iostream;
};

class Instruction
{
public:
  using IndexType = size_t;

  virtual ~Instruction()                      = default;
  virtual Address execute( Environment& env ) = 0;
};

struct NoOpInstruction : Instruction
{
  Address calc_next_instruction_address( const Address addr ) const
  {
    return addr + Address( 1 );
  }
};

struct OneOpInstruction : Instruction
{
  OneOpInstruction( const Word wa ) : a( wa )
  {
  }

  Address calc_next_instruction_address( const Address addr ) const
  {
    return addr + Address( 2 );
  }

protected:
  Word a;
};

struct TwoOpsInstruction : Instruction
{
  TwoOpsInstruction( const Word wa, const Word wb ) : a( wa ), b( wb )
  {
  }

  Address calc_next_instruction_address( const Address addr ) const
  {
    return addr + Address( 3 );
  }

protected:
  Word a;
  Word b;
};

struct ThreeOpsInstruction : Instruction
{
  ThreeOpsInstruction( const Word wa, const Word wb, const Word wc ) : a( wa ), b( wb ), c( wc )
  {
  }

  Address calc_next_instruction_address( const Address addr ) const
  {
    return addr + Address( 4 );
  }

protected:
  Word a;
  Word b;
  Word c;
};

std::unique_ptr<Instruction> read_instruction( const MemoryStorage&, Address command_address );

}  // namespace synacor
