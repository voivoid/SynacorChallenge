#pragma once

#include "synacor/instructions_base.h"

#include <iosfwd>

namespace synacor
{

namespace instructions
{

void set_ostream( std::ostream* );
void set_istream( std::istream* );

struct Set : TwoOpsInstruction
{
public:
  using TwoOpsInstruction::TwoOpsInstruction;
  static constexpr IndexType index = { 1 };
  virtual Address execute( MemoryStorage&, Stack&, Address current_address ) override;
};

struct Push : OneOpInstruction
{
public:
  using OneOpInstruction::OneOpInstruction;
  static constexpr IndexType index = { 2 };
  virtual Address execute( MemoryStorage&, Stack&, Address current_address ) override;
};

struct Pop : OneOpInstruction
{
public:
  using OneOpInstruction::OneOpInstruction;
  static constexpr IndexType index = { 3 };
  virtual Address execute( MemoryStorage&, Stack&, Address current_address ) override;
};

struct Add : ThreeOpsInstruction
{
public:
  using ThreeOpsInstruction::ThreeOpsInstruction;
  static constexpr IndexType index = { 9 };
  virtual Address execute( MemoryStorage&, Stack&, Address current_address ) override;
};

struct Out : OneOpInstruction
{
public:
    using OneOpInstruction::OneOpInstruction;
    static constexpr IndexType index = { 19 };
    virtual Address execute( MemoryStorage&, Stack&, Address current_address ) override;
};

struct Noop : NoOpInstruction
{
public:
  static constexpr IndexType index = { 21 };
  virtual Address execute( MemoryStorage&, Stack&, Address current_address ) override;
};

}  // namespace instructions

}  // namespace synacor
