# FMI Standard Check based on LLVM

This project implements an FMI orchestration analysis based on the FMI standard. It checks if the standard calls are used correctly according to the standard.



# Build instructons

## Rewuirements

* clang
* LLVM 15
* cmake
* make or ninja


# Build

cmake -B build -S. -DLT_LLVM_INSTALL_DIR=<path to llvm install>
make  -C build
# to build the LLVM ir in a readable format use
make -C build sim_dis sim_c_dis
