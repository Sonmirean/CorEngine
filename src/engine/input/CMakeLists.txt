file(GLOB_RECURSE PUBLIC_SOURCE "*.cpp")
target_include_directories(CorEngine PUBLIC 
"include/engine/input"
)
add_library("input" STATIC ${PUBLIC_SOURCE})