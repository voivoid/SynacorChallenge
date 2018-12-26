#pragma once

#include "synacor/arch.h"
#include <stack>

namespace synacor
{

class Stack
{
public:
  void push( Word word );
  void pop();

  Word top() const;

  bool is_empty() const;

private:
  std::stack<Word> stack;
};

}  // namespace synacor
