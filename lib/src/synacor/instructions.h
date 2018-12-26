#pragma once

#include "synacor/instructions_base.h"

namespace synacor
{

namespace instructions
{

class Set : TwoOpsInstruction
{
public:
  using TwoOpsInstruction::TwoOpsInstruction;
  static constexpr IndexType index = { 1 };

  virtual void execute( MemoryStorage&, Stack& );
};

class Add : ThreeOpsInstruction
{
public:
  using ThreeOpsInstruction::ThreeOpsInstruction;
  static constexpr IndexType index = { 9 };

  virtual void execute( MemoryStorage&, Stack& );
};

class Noop : Instruction
{
public:
  static constexpr IndexType index = { 21 };

  virtual void execute( MemoryStorage&, Stack& );
};

}  // namespace instructions

}  // namespace synacor
