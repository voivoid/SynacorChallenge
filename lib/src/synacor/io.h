#pragma once

#include "synacor/interfaces.h"

#include <iosfwd>

namespace synacor
{
class IO : public IStream
{
public:
  IO( std::istream& input, std::ostream& output );

  virtual void put( char c ) override;
  virtual char get() override;

private:
  std::istream& in;
  std::ostream& out;
};
}  // namespace synacor
