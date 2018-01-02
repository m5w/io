add_library (decode decode.cc)
target_compile_definitions (decode PUBLIC $<$<CONFIG:Debug>:ENABLE_DEBUG>)
add_library (encode encode.cc)
target_compile_definitions (encode PUBLIC $<$<CONFIG:Debug>:ENABLE_DEBUG>)
