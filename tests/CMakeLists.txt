add_executable (testio testio.cc)
target_link_libraries (testio ${Boost_LIBRARIES} decode encode)
target_compile_definitions (testio PUBLIC $<$<CONFIG:Debug>:ENABLE_DEBUG>)
