#pragma once

#include "synacor/arch.h"

#include <cstddef>

namespace synacor
{

class MemoryStorage;
class Stack;

class Instruction
{
public:
  using IndexType = size_t;

  virtual ~Instruction()                         = default;
  virtual void execute( MemoryStorage&, Stack& ) = 0;
};

struct OneOpInstruction : Instruction
{
public:
  OneOpInstruction( const Word wa ) : a( wa )
  {
  }

protected:
  Word a;
};

struct TwoOpsInstruction : Instruction
{
public:
  TwoOpsInstruction( const Word wa, const Word wb ) : a( wa ), b( wb )
  {
  }

protected:
  Word a;
  Word b;
};

struct ThreeOpsInstruction : Instruction
{
public:
  ThreeOpsInstruction( const Word wa, const Word wb, const Word wc ) : a( wa ), b( wb ), c( wc )
  {
  }

protected:
  Word a;
  Word b;
  Word c;
};

std::unique_ptr<Instruction> read_instruction( const MemoryStorage&, Address command_address );

}
