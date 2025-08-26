file(GLOB_RECURSE PUBLIC_SOURCE "*.cpp")
target_include_directories(CorEngine PUBLIC 
"include/engine/physics"
)
add_library(physics STATIC ${PUBLIC_SOURCE})