#pragma once

#include "synacor/instructions_base.h"

namespace synacor
{

namespace instructions
{

struct Set : TwoOpsInstruction
{
public:
  using TwoOpsInstruction::TwoOpsInstruction;
  static constexpr IndexType index = { 1 };
  virtual Address execute( MemoryStorage&, Stack&, Address current_address );
};

struct Push : OneOpInstruction
{
public:
  using OneOpInstruction::OneOpInstruction;
  static constexpr IndexType index = { 2 };
  virtual Address execute( MemoryStorage&, Stack&, Address current_address );
};

struct Pop : OneOpInstruction
{
public:
  using OneOpInstruction::OneOpInstruction;
  static constexpr IndexType index = { 3 };
  virtual Address execute( MemoryStorage&, Stack&, Address current_address );
};

struct Add : ThreeOpsInstruction
{
public:
  using ThreeOpsInstruction::ThreeOpsInstruction;
  static constexpr IndexType index = { 9 };
  virtual Address execute( MemoryStorage&, Stack&, Address current_address );
};

struct Noop : NoOpInstruction
{
public:
  static constexpr IndexType index = { 21 };
  virtual Address execute( MemoryStorage&, Stack&, Address current_address );
};

}  // namespace instructions

}  // namespace synacor
