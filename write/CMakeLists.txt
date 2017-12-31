add_library (write write.cc)
target_compile_definitions (write PUBLIC $<$<CONFIG:Debug>:ENABLE_DEBUG>)
add_library (read read.cc)
target_compile_definitions (read PUBLIC $<$<CONFIG:Debug>:ENABLE_DEBUG>)
