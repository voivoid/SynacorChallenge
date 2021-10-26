add_executable(SynacorTests
  main.cpp
  instructions.cpp
  machine.cpp
  memory_storage.cpp
  stack.cpp
  test_utils.h)
target_include_directories(SynacorTests PRIVATE ../lib/src)
target_compile_options(SynacorTests PRIVATE ${SYNACOR_CXX_WARNINGS})
target_link_libraries(SynacorTests PRIVATE SynacorLib Boost::boost Boost::unit_test_framework typesafe)

add_test(NAME SynacorTests COMMAND $<TARGET_FILE:SynacorTests>)
