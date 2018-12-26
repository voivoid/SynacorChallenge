#include "synacor/memory_storage.h"

#include "synacor/assert.h"

namespace
{
static const size_t max_number = static_cast<size_t>( std::numeric_limits<synacor::Number>::max() );
}

namespace synacor
{

MemoryStorage::MemoryStorage() : storage( new StorageData{} )
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
  return Address( static_cast<Word>( max_number + 1 + index ) );
}

size_t MemoryStorage::get_word_index( Address address ) const
{
  return static_cast<size_t>( type_safe::get( address ) );
}

bool is_number( const Word word )
{
  return word <= max_number;
}

bool is_register( const Word word )
{
  return !is_number( word ) && word <= max_number + RegistersCount;
}

bool is_valid_value( const Word word )
{
  return is_number( word ) || is_register( word );
}

bool is_valid_address( const Address address )
{
  return is_valid_value( Word( address ) );
}

Number get_value( MemoryStorage& memory, Word word )
{
  if ( is_number( word ) )
  {
    return word;
  }
  else if ( is_register( word ) )
  {
    return memory.read( Address( word ) );
  }
  else
  {
    SYNACOR_ENSURE( false );
  }
}

}  // namespace synacor
