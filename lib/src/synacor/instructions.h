#pragma once

#include <cstddef>

namespace synacor
{

class AddressSpace;
class Stack;
class Registers;

namespace instructions
{

class Instruction
{
public:
  using IndexType = size_t;

  virtual ~Instruction()                                    = default;
  virtual void execute( AddressSpace&, Stack&, Registers& ) = 0;
};

class Set : Instruction
{
public:
  static constexpr IndexType index = { 1 };

  virtual void execute( AddressSpace&, Stack&, Registers& );
};

}  // namespace instructions

}  // namespace synacor
