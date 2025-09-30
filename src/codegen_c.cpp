#include "codegen_c.hpp"
#include <cstdlib>
#include <filesystem>

void compileConv2D(const std::string &target) {
    std::filesystem::create_directory("build");
    std::string cmd;
    if (target == "x86") {
        cmd = "clang -O3 -c src/kernels/conv2d.c -o build/conv2d_x86.o";
    } else {
        cmd = "arm-none-eabi-clang -O3 -target arm-none-eabi -mcpu=cortex-m4 -c src/kernels/conv2d.c -o build/conv2d_arm.o";
    }
    system(cmd.c_str());
}
