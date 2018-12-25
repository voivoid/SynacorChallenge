#pragma once

#include "synacor/arch.h"
#include "synacor/assert.h"

#include "boost/multiprecision/cpp_int.hpp"

#include <variant>

namespace synacor
{

bool is_number( Word word );
bool is_register( Word word );
bool is_valid( Word word );

struct InvalidValue
{
};

using Number = boost::multiprecision::number<
    boost::multiprecision::cpp_int_backend<BitsInNumber, BitsInNumber, boost::multiprecision::unsigned_magnitude>>;

struct Register
{
  Register( size_t idx ) : index( idx )
  {
    SYNACOR_ASSERT( index < RegistersCount );
  }
  size_t index;
};

using Value = std::variant<Number, Register, InvalidValue>;

Value interpret_word( Word word );

}  // namespace synacor
