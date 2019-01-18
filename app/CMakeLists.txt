add_executable(SynacorApp
  main.cpp
  cmd.h
  cmd_parser.h
  cmd_parser.cpp
  cmd_runner.h
  cmd_runner.cpp)
target_compile_options(SynacorApp PRIVATE ${SYNACOR_CXX_WARNINGS})
target_link_libraries(SynacorApp PRIVATE SynacorLib Boost::boost Boost::program_options Boost::iostreams)
