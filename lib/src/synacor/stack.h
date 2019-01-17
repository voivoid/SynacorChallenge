#pragma once

#include "synacor/arch.h"
#include "synacor/interfaces.h"
#include <stack>

namespace synacor
{

class Stack : public IStack
{
public:
  Stack() = default;
  Stack( const Stack& rhs );

  virtual void push( Number word ) override;
  virtual Number pop() override;

  Number top() const;

  virtual bool is_empty() const override;

  friend bool operator==( const Stack& s1, const Stack& s2 );

private:
  std::stack<Number> stack;
};

}  // namespace synacor
