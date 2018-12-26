#pragma once

#include "synacor/arch.h"

#include <array>
#include <memory>

namespace synacor
{

class MemoryStorage
{
public:
  MemoryStorage();

  Word read( Address address ) const;
  void store( Address address, Word number );

  Address get_register( size_t index ) const;

private:
  size_t get_word_index( Address address ) const;

private:
  using StorageData = std::array<Word, AddressSpaceSize + RegistersCount>;
  std::unique_ptr<StorageData> storage;
};

bool is_number( Word word );
bool is_register( Word word );
bool is_valid_value( Word word );
bool is_valid_address( Address address );

Number get_value( MemoryStorage& memory, Word word );


}  // namespace synacor
