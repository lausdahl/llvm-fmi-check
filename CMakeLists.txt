cmake_minimum_required(VERSION 3.13.4)
project(llvm-fmi-check)


add_subdirectory(analysis)

# ONLY add one of the two subdirs
# add_subdirectory(examples/initialize_singleWaterTank/cpp)
add_subdirectory(examples/drobotti/cpp)

add_subdirectory(examples/fmicall)
