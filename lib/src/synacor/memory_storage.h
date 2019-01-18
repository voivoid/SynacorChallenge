#pragma once

#include "synacor/arch.h"
#include "synacor/assert.h"
#include "synacor/interfaces.h"

#include <algorithm>
#include <array>
#include <memory>

namespace synacor
{

class MemoryStorage : public IMemory
{
public:
  MemoryStorage();
  MemoryStorage( const MemoryStorage& rhs );

  virtual Word read( Address address ) const override;
  virtual void store( Address address, Word number ) override;

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


}  // namespace synacor
