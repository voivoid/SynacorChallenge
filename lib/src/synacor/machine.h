#pragma once

#include "interfaces.h"

#include <iosfwd>
#include <memory>

namespace synacor
{

class MemoryStorage;
class Stack;
class IO;
struct Machine
{
  IMemory& memory;
  IStack& stack;
  IStream& io;
};

void run( Machine& machine );

}  // namespace synacor
