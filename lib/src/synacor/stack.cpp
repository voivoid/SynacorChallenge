#include "synacor/stack.h"

#include "synacor/assert.h"

namespace synacor
{
void Stack::push( Word word )
{
  stack.push( word );
}

void Stack::pop()
{
  SYNACOR_ENSURE( !is_empty() );
  stack.pop();
}

bool Stack::is_empty() const
{
  return stack.empty();
}

Word Stack::top() const
{
  SYNACOR_ENSURE( !is_empty() );
  return stack.top();
}

}  // namespace synacor
