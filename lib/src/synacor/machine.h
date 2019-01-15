#pragma once

#include "synacor/arch.h"

#include <iosfwd>
#include <memory>

namespace synacor
{

class MemoryStorage;
class Stack;
struct Machine
{
  std::unique_ptr<MemoryStorage> memory;
  std::unique_ptr<Stack> stack;
  Address current_address;
  std::istream& istream;
  std::ostream& ostream;
};

void run( Machine& machine );

}  // namespace synacor
