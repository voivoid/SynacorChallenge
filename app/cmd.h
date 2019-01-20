#pragma once

#include <string>
#include <variant>

namespace synacor
{
struct RunBinaryCmd
{
  std::string filepath;
};

struct ShowHelpCmd
{
  std::string help_message;
};

using Cmd = std::variant<RunBinaryCmd, ShowHelpCmd>;

}  // namespace synacor
