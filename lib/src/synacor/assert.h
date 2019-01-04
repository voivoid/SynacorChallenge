#pragma once

#include <cassert>
#include <stdexcept>

#ifdef NDEBUG
#  define SYNACOR_ENSURE( expr ) if ( !expr ) { throw std::runtime_error(); }
#else
#  define SYNACOR_ENSURE( expr ) assert( expr )
#endif
