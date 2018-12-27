#pragma once

#include "synacor/arch.h"
#include <stack>

namespace synacor
{

class Stack
{
public:
  Stack() = default;
  Stack( const Stack& rhs );

  void push( Number word );
  Number pop();

  Number top() const;

  bool is_empty() const;

  friend bool operator==( const Stack& s1, const Stack& s2 );

private:
  std::stack<Number> stack;
};

}  // namespace synacor
