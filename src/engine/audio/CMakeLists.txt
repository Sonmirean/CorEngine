file(GLOB_RECURSE PUBLIC_SOURCE "*.cpp")
target_include_directories(CorEngine PUBLIC 
"include/engine/audio"
)
add_library(audio STATIC ${PUBLIC_SOURCE})