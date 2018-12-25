#pragma once

#include "synacor/arch.h"
#include "synacor/value.h"

#include <array>

namespace synacor
{

class Registers
{
public:
  Registers();

  Number get_register_value( Register reg );
  void set_register_value( Register reg, Number value );

private:
  Number& get_register( Register reg_num );

private:
  std::array<Number, RegistersCount> registers;
};

}  // namespace synacor
