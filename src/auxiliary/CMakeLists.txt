file(GLOB_RECURSE PUBLIC_SOURCE "*.cpp")
target_include_directories(CorEngine PUBLIC 
"include/auxiliary"
)
add_library(auxiliary STATIC ${PUBLIC_SOURCE})