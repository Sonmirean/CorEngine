file(GLOB_RECURSE PUBLIC_SOURCE "*.cpp")
target_include_directories(CorEngine PUBLIC 
"include/engine/core/math"
)
add_library(math STATIC ${PUBLIC_SOURCE})