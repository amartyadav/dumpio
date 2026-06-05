cmake_minimum_required(VERSION 4.2)
project(dumpio)

set(CMAKE_CXX_STANDARD 20)

# CHANGE: Add library.h here so IDEs explicitly track it as part of the library
add_library(dumpio SHARED library.cpp library.h)

# Adding installation rules
install(TARGETS dumpio LIBRARY DESTINATION lib)
install(FILES library.h DESTINATION include/dumpio)
