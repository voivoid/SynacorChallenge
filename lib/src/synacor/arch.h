#pragma once

#include "boost/multiprecision/cpp_int.hpp"

#include "type_safe/strong_typedef.hpp"

#include <cstddef>
#include <cstdint>

namespace synacor
{

static constexpr size_t BitsInNumber     = 15;
static constexpr size_t RegistersCount   = 8;
static constexpr size_t AddressSpaceSize = 32768;


using Word = std::uint16_t;

using Number = boost::multiprecision::number<
    boost::multiprecision::cpp_int_backend<BitsInNumber, BitsInNumber, boost::multiprecision::unsigned_magnitude>>;

struct Address : type_safe::strong_typedef<Address, Word>,
                 type_safe::strong_typedef_op::integer_arithmetic<Address>,
                 type_safe::strong_typedef_op::equality_comparison<Address>
{
  using strong_typedef::strong_typedef;
};

}  // namespace synacor
