add_executable (testwrite testwrite.cc)
target_link_libraries (testwrite ${Boost_LIBRARIES} write)
target_compile_definitions (testwrite PUBLIC $<$<CONFIG:Debug>:ENABLE_DEBUG>)
