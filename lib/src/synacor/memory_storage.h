#pragma once

#include "synacor/arch.h"
#include "synacor/assert.h"

#include <array>
#include <memory>
#include <algorithm>

namespace synacor
{

class MemoryStorage
{
public:
  MemoryStorage();
  MemoryStorage( const MemoryStorage& rhs );

  Word read( Address address ) const;
  void store( Address address, Word number );

  Address get_register( size_t index ) const;

  template <typename Iter>
  void load_memory( Iter begin, Iter end )
  {
      SYNACOR_ENSURE( static_cast<size_t>( std::distance( begin, end ) ) <= AddressSpaceSize );
      std::copy( begin, end, storage->begin() );
  }

  friend bool operator==( const MemoryStorage& m1, const MemoryStorage& m2 );

private:
  size_t get_word_index( Address address ) const;

private:
  using StorageData = std::array<Word, AddressSpaceSize + RegistersCount>;
  std::unique_ptr<StorageData> storage;
};

bool is_number( Word word );
bool is_register( Word word );
bool is_valid_address( Address address );

Number get_value( MemoryStorage& memory, Word word );


}  // namespace synacor
