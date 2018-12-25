#pragma once

#include "synacor/arch.h"
#include "synacor/assert.h"

#include "boost/multiprecision/cpp_int.hpp"
#include "type_safe/strong_typedef.hpp"

#include <variant>

namespace synacor
{

using Number = boost::multiprecision::number<
    boost::multiprecision::cpp_int_backend<NumberBits, NumberBits, boost::multiprecision::unsigned_magnitude>>;
;

struct Register
{
  Register( size_t index ) : index( index )
  {
    SYNACOR_ASSERT( index < RegistersCount );
  }
  size_t index;
};

using Value = std::variant<Number, Register>;

Value interpret_word( Word word );

}  // namespace synacor
