#include <iostream>
#include "codegen_ir.hpp"
#include "codegen_c.hpp"

int main(int argc, char** argv) {
    std::string target = "x86";
    if (argc > 1) {
        target = argv[1];
    }

    std::cout << "🔨 Building for target: " << target << std::endl;

    // Example: generate Sigmoid function in LLVM IR
    generateSigmoidIR("sigmoid.ll");

    // Compile Conv2D kernel
    compileConv2D(target);

    return 0;
}
