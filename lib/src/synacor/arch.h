#pragma once

#include <cstddef>
#include <cstdint>

namespace synacor
{

using Word                               = std::uint16_t;
static constexpr size_t BitsInNumber     = 15;
static constexpr size_t RegistersCount   = 8;
static constexpr size_t AddressSpaceSize = 32768;

}  // namespace synacor
