add_library (write write.cc)
target_compile_definitions (write PUBLIC $<$<CONFIG:Debug>:ENABLE_DEBUG>)
