add_library(SynacorLib
  inc/synacor/synacor.h

  src/synacor/arch.h
  src/synacor/assert.h
  src/synacor/instructions.h
  src/synacor/instructions.cpp
  src/synacor/instructions_base.h
  src/synacor/instructions_base.cpp
  src/synacor/interfaces.h
  src/synacor/machine.h
  src/synacor/machine.cpp
  src/synacor/memory_storage.h
  src/synacor/memory_storage.cpp
  src/synacor/stack.h
  src/synacor/stack.cpp
  src/synacor/synacor.cpp)
target_compile_options(SynacorLib PRIVATE ${SYNACOR_CXX_WARNINGS})
target_include_directories(SynacorLib PUBLIC inc PRIVATE src)
target_link_libraries(SynacorLib PRIVATE Boost::boost typesafe)
