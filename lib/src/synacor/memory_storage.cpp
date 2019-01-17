#include "synacor/memory_storage.h"

#include "synacor/assert.h"

namespace synacor
{

MemoryStorage::MemoryStorage() : storage( new StorageData{} )
{
}

MemoryStorage::MemoryStorage( const MemoryStorage& rhs ) : storage( new StorageData{ *rhs.storage.get() } )
{
}

Word MemoryStorage::read( Address address ) const
{
  SYNACOR_ENSURE( is_valid_address( address ) );
  return ( *storage )[ get_word_index( address ) ];
}

void MemoryStorage::store( Address address, Word word )
{
  SYNACOR_ENSURE( is_valid_address( address ) );
  ( *storage )[ get_word_index( address ) ] = word;
}

Address MemoryStorage::get_register( size_t index ) const
{
  SYNACOR_ENSURE( index < RegistersCount );
  return Address( static_cast<Word>( NumberMax + 1 + index ) );
}

size_t MemoryStorage::get_word_index( Address address ) const
{
  return static_cast<size_t>( type_safe::get( address ) );
}

bool operator==( const MemoryStorage& m1, const MemoryStorage& m2 )
{
  return *m1.storage == *m2.storage;
}

}  // namespace synacor
