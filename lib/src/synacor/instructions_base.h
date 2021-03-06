#pragma once

#include "synacor/arch.h"
#include "synacor/machine.h"

#include <cstddef>

namespace synacor
{

class MemoryStorage;
class Stack;

class Instruction
{
public:
  using IndexType = size_t;

  virtual ~Instruction()                                               = default;
  virtual Address execute( Machine& machine, Address current_address ) = 0;
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

std::unique_ptr<Instruction> read_instruction( const IMemoryReader& memory_reader, Address command_address );

}  // namespace synacor
