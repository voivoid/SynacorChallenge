#pragma once

#include "boost/multiprecision/cpp_int.hpp"
#include "synacor/arch.h"

namespace synacor
{

bool is_number( Word word );
bool is_register( Word word );
bool is_valid( Word word );

using Number = boost::multiprecision::number<
    boost::multiprecision::cpp_int_backend<BitsInNumber, BitsInNumber, boost::multiprecision::unsigned_magnitude>>;


}  // namespace synacor