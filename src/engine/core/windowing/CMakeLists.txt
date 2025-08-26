file(GLOB_RECURSE PUBLIC_SOURCE "*.cpp")
target_include_directories(CorEngine PUBLIC 
"include/engine/core"
"include/engine/core/math"
"include/engine/core/windowing"
)
add_library(windowing STATIC ${PUBLIC_SOURCE})