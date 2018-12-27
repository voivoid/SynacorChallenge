#include "synacor/stack.h"

#include "synacor/assert.h"

namespace synacor
{

Stack::Stack( const Stack& rhs ) : stack( rhs.stack )
{
}

void Stack::push( Number word )
{
  stack.push( word );
}

Number Stack::pop()
{
  SYNACOR_ENSURE( !is_empty() );
  const auto n = top();
  stack.pop();
  return n;
}

bool Stack::is_empty() const
{
  return stack.empty();
}

Number Stack::top() const
{
  SYNACOR_ENSURE( !is_empty() );
  return stack.top();
}

bool operator==( const Stack& s1, const Stack& s2 )
{
  return s1.stack == s2.stack;
}

}  // namespace synacor
