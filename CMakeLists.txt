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

add_subdirectory(lib)
add_subdirectory(app)

enable_testing()
add_subdirectory(tests)

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
