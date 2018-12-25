cmake_minimum_required(VERSION 3.13)
project(Synacor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/thirdparties/boost.cmake)
include(cmake/thirdparties/rangev3.cmake)
include(cmake/thirdparties/typesafe.cmake)

add_library(SynacorLib
  lib/inc/synacor/synacor.h
  lib/src/synacor/synacor.cpp
  lib/src/synacor/assert.h
  lib/src/synacor/arch.h
  lib/src/synacor/registers.h
  lib/src/synacor/registers.cpp
  lib/src/synacor/stack.h
  lib/src/synacor/stack.cpp
  lib/src/synacor/address_space.h
  lib/src/synacor/address_space.cpp
  lib/src/synacor/instructions.h
  lib/src/synacor/instructions.cpp
  lib/src/synacor/value.h
  lib/src/synacor/value.cpp)
target_include_directories(SynacorLib PUBLIC lib/inc PRIVATE lib/src)
target_link_libraries(SynacorLib PRIVATE Boost::boost typesafe)

add_executable(SynacorApp
  app/main.cpp)
target_link_libraries(SynacorApp PRIVATE SynacorLib Boost::boost Boost::program_options)



add_executable(SynacorTests
  tests/main.cpp
  tests/instructions.cpp
  tests/registers.cpp)
target_include_directories(SynacorTests PRIVATE lib/src)
target_link_libraries(SynacorTests PRIVATE SynacorLib Boost::boost Boost::unit_test_framework typesafe)

find_program(ClangFormat clang-format)
if(ClangFormat)
  set(SynacorTargets SynacorLib SynacorApp SynacorTests)
  foreach(SynacorTarget ${SynacorTargets})
    get_target_property(SynacorTargetSources ${SynacorTarget} SOURCES)
    get_target_property(SynacorTargetDir ${SynacorTarget} SOURCE_DIR)
    foreach(Src ${SynacorTargetSources})
      list(APPEND SynacorSources "${SynacorTargetDir}/${Src}")
    endforeach()
  endforeach()

  list(FILTER SynacorSources INCLUDE REGEX "^.*\.(h|cpp)\$")

  add_custom_target(clangformat
    COMMAND clang-format -style=file -i ${SynacorSources}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()


enable_testing()
add_test(NAME AocUtilsTests COMMAND $<TARGET_FILE:SynacorTests>)
