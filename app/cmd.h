#pragma once

#include <string>
#include <variant>

namespace synacor
{
struct RunBinary
{
  std::string filepath;
};

struct ShowHelp
{
};

struct UnknownCmd
{
};

using Cmd = std::variant<RunBinary, ShowHelp, UnknownCmd>;

}  // namespace synacor
