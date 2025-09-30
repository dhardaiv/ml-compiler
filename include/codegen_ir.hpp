#pragma once
#include <string>

// Emit LLVM IR for elementwise ops
void generateSigmoidIR(const std::string &outfile);
void generateSoftmaxIR(const std::string &outfile);
