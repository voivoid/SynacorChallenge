#pragma once

#include <cassert>
#include <stdexcept>

#include "boost/preprocessor/stringize.hpp"

namespace synacor
{
struct assertion : std::runtime_error
{
  using std::runtime_error::runtime_error;
};
}  // namespace synacor

#define SYNACOR_ENSURE( expr )                                                                                                             \
  if ( !( expr ) )                                                                                                                         \
  {                                                                                                                                        \
    throw synacor::assertion{ #expr " in file " __FILE__ " in line " BOOST_PP_STRINGIZE( __LINE__ ) };                                     \
  }

#define SYNACOR_ASSERT( expr ) assert( expr )
