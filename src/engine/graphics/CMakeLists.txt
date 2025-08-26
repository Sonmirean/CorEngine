file(GLOB_RECURSE PUBLIC_SOURCE "*.cpp")
target_include_directories(CorEngine PUBLIC 
"include/engine/graphics"
)
add_library("graphics" ${PUBLIC_SOURCE})