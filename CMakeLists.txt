cmake_minimum_required(VERSION 3.13)
project(Synacor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/thirdparties/boost.cmake)
include(cmake/thirdparties/typesafe.cmake)

if(MSVC)
  set(SYNACOR_CXX_WARNINGS /W4 /WX)
else()
  set(SYNACOR_CXX_WARNINGS -Wall -Wextra -Wpedantic -Werror -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wrestrict -Wnull-dereference -Wold-style-cast -Wuseless-cast -Wdouble-promotion -Wshadow -Wformat=2 -Wconversion -Wsign-conversion -Wfloat-equal -Wcast-qual -Winit-self -Wpointer-arith -Wswitch -Wswitch-enum -Woverloaded-virtual -Wundef)
endif()

add_library(SynacorLib
  lib/inc/synacor/synacor.h

  lib/src/synacor/arch.h
  lib/src/synacor/assert.h
  lib/src/synacor/instructions.h
  lib/src/synacor/instructions.cpp
  lib/src/synacor/instructions_base.h
  lib/src/synacor/instructions_base.cpp
  lib/src/synacor/machine.h
  lib/src/synacor/machine.cpp
  lib/src/synacor/memory_storage.h
  lib/src/synacor/memory_storage.cpp
  lib/src/synacor/stack.h
  lib/src/synacor/stack.cpp
  lib/src/synacor/synacor.cpp)
target_compile_options(SynacorLib PRIVATE ${SYNACOR_CXX_WARNINGS})
target_include_directories(SynacorLib PUBLIC lib/inc PRIVATE lib/src)
target_link_libraries(SynacorLib PRIVATE Boost::boost typesafe)

add_executable(SynacorApp
  app/main.cpp)
target_compile_options(SynacorApp PRIVATE ${SYNACOR_CXX_WARNINGS})
target_link_libraries(SynacorApp PRIVATE SynacorLib Boost::boost Boost::program_options)



add_executable(SynacorTests
  tests/main.cpp
  tests/instructions.cpp
  tests/machine.cpp
  tests/memory_storage.cpp
  tests/stack.cpp
  tests/test_utils.h)
target_include_directories(SynacorTests PRIVATE lib/src tests)
target_compile_options(SynacorTests PRIVATE ${SYNACOR_CXX_WARNINGS})
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

if(MSVC)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SynacorTests)
endif()


enable_testing()
add_test(NAME AocUtilsTests COMMAND $<TARGET_FILE:SynacorTests>)
