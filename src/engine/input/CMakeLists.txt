file(GLOB_RECURSE PUBLIC_SOURCE "*.cpp")
add_library("input" STATIC ${PUBLIC_SOURCE})