file(GLOB_RECURSE PUBLIC_SOURCE "*.cpp")
target_include_directories(CorEngine PUBLIC 
"include/auxiliary"
"include/engine/core"
"include/engine/core/windowing"
)
add_library(core STATIC ${PUBLIC_SOURCE})