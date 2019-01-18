#pragma once

#include <cstddef>
#include <iosfwd>

namespace synacor
{
void execute( const std::byte* memory_data, size_t memory_data_size, std::istream& istream, std::ostream& ostream );
}
