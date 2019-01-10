#pragma once

#include "synacor/arch.h"

#include <iosfwd>

namespace synacor
{

class MemoryStorage;
class Stack;
struct Machine
{
  MemoryStorage& memory;
  Stack& stack;
  Address current_address;
  std::istream& istream;
  std::ostream& ostream;
};

void execute( Machine& machine );

}  // namespace synacor
