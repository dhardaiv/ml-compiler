#include "codegen_ir.hpp"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"

using namespace llvm;

void generateSigmoidIR(const std::string &outfile) {
    LLVMContext Context;
    Module *M = new Module("sigmoid_module", Context);
    IRBuilder<> Builder(Context);

    // Signature: void sigmoid(double* arr, int n)
    FunctionType *FT = FunctionType::get(Type::getVoidTy(Context),
                                         {Type::getDoublePtrTy(Context), Type::getInt32Ty(Context)},
                                         false);
    Function *F = Function::Create(FT, Function::ExternalLinkage, "sigmoid", M);

    auto args = F->arg_begin();
    Value *arr = args++;
    arr->setName("arr");
    Value *n = args++;
    n->setName("n");

    BasicBlock *EntryBB = BasicBlock::Create(Context, "entry", F);
    Builder.SetInsertPoint(EntryBB);

    // loop i = 0
    Value *iVar = Builder.CreateAlloca(Type::getInt32Ty(Context), nullptr, "i");
    Builder.CreateStore(ConstantInt::get(Type::getInt32Ty(Context), 0), iVar);

    BasicBlock *LoopBB = BasicBlock::Create(Context, "loop", F);
    BasicBlock *AfterBB = BasicBlock::Create(Context, "after", F);

    Builder.CreateBr(LoopBB);
    Builder.SetInsertPoint(LoopBB);

    Value *iVal = Builder.CreateLoad(Type::getInt32Ty(Context), iVar, "iVal");
    Value *cond = Builder.CreateICmpSLT(iVal, n);
    Builder.CreateCondBr(cond, LoopBB, AfterBB);

    // access arr[i]
    Value *ptr = Builder.CreateGEP(Type::getDoubleTy(Context), arr, iVal);
    Value *val = Builder.CreateLoad(Type::getDoubleTy(Context), ptr, "val");

    // sigmoid = 1 / (1 + exp(-x))
    Function *expFunc = Intrinsic::getDeclaration(M, Intrinsic::exp, {Type::getDoubleTy(Context)});
    Value *neg = Builder.CreateFNeg(val, "neg");
    Value *expv = Builder.CreateCall(expFunc, {neg}, "expv");
    Value *denom = Builder.CreateFAdd(ConstantFP::get(Type::getDoubleTy(Context), 1.0), expv);
    Value *sigm = Builder.CreateFDiv(ConstantFP::get(Type::getDoubleTy(Context), 1.0), denom, "sigm");

    Builder.CreateStore(sigm, ptr);

    // i++
    Value *inc = Builder.CreateAdd(iVal, ConstantInt::get(Type::getInt32Ty(Context), 1));
    Builder.CreateStore(inc, iVar);
    Builder.CreateBr(LoopBB);

    Builder.SetInsertPoint(AfterBB);
    Builder.CreateRetVoid();

    // Verify and dump IR
    verifyFunction(*F);
    std::error_code EC;
    raw_fd_ostream OS(outfile, EC, sys::fs::OF_None);
    M->print(OS, nullptr);
}
