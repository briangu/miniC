#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/Analysis/BasicAliasAnalysis.h>
#include <llvm/Analysis/Passes.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>

#include <iostream>

using namespace llvm;

int main() {
  LLVMContext Context;
  std::unique_ptr<Module> TheModule =
      std::make_unique<Module>("my_module", Context);
  IRBuilder<> Builder(Context);

  // Create the function types
  FunctionType *funcType = FunctionType::get(
      Type::getDoubleTy(Context),
      {Type::getDoubleTy(Context), Type::getDoubleTy(Context)}, false);

  // Create the functions
  Function *addFunc = Function::Create(funcType, Function::ExternalLinkage,
                                       "add", TheModule.get());
  Function *subFunc = Function::Create(funcType, Function::ExternalLinkage,
                                       "sub", TheModule.get());
  Function *mulFunc = Function::Create(funcType, Function::ExternalLinkage,
                                       "mul", TheModule.get());
  Function *divFunc = Function::Create(funcType, Function::ExternalLinkage,
                                       "div", TheModule.get());

  // Create the BasicBlocks
  BasicBlock *addBB = BasicBlock::Create(Context, "entry", addFunc);
  BasicBlock *subBB = BasicBlock::Create(Context, "entry", subFunc);
  BasicBlock *mulBB = BasicBlock::Create(Context, "entry", mulFunc);
  BasicBlock *divBB = BasicBlock::Create(Context, "entry", divFunc);

  // Create the IRBuilder for each function
  Builder.SetInsertPoint(addBB);
  Value *addResult =
      Builder.CreateFAdd(addFunc->arg_begin(), (addFunc->arg_begin() + 1));
  Builder.CreateRet(addResult);

  Builder.SetInsertPoint(subBB);
  Value *subResult =
      Builder.CreateFSub(subFunc->arg_begin(), (subFunc->arg_begin() + 1));
  Builder.CreateRet(subResult);

  Builder.SetInsertPoint(mulBB);
  Value *mulResult =
      Builder.CreateFMul(mulFunc->arg_begin(), (mulFunc->arg_begin() + 1));
  Builder.CreateRet(mulResult);

  Builder.SetInsertPoint(divBB);
  Value *divResult =
      Builder.CreateFDiv(divFunc->arg_begin(), (divFunc->arg_begin() + 1));
  Builder.CreateRet(divResult);

  // Verify the module
  if (verifyModule(*TheModule, &errs())) {
    std::cerr << "TheModule is broken!" << std::endl;
    return 1;
  }

  // Initialize the native target and select the appropriate execution engine
  InitializeNativeTarget();
  InitializeNativeTargetAsmPrinter();

  // Create the execution engine
  std::string Error;
  ExecutionEngine *EE = EngineBuilder(std::move(TheModule))
                            .setErrorStr(&Error)
                            .setEngineKind(EngineKind::JIT)
                            .create();

  if (!EE) {
    std::cerr << "Failed to create ExecutionEngine: " << Error << std::endl;
    return 1;
  }

  // Define the function pointer type for the arithmetic functions
  using ArithmeticFuncPtr = double (*)(double, double);

  // Obtain the addresses of the compiled functions
  uintptr_t addFuncAddr = EE->getFunctionAddress(addFunc->getName().str());
  uintptr_t subFuncAddr = EE->getFunctionAddress(subFunc->getName().str());
  uintptr_t mulFuncAddr = EE->getFunctionAddress(mulFunc->getName().str());
  uintptr_t divFuncAddr = EE->getFunctionAddress(divFunc->getName().str());

  // Cast the addresses to the function pointer type
  ArithmeticFuncPtr addFuncPtr = reinterpret_cast<ArithmeticFuncPtr>(addFuncAddr);
  ArithmeticFuncPtr subFuncPtr = reinterpret_cast<ArithmeticFuncPtr>(subFuncAddr);
  ArithmeticFuncPtr mulFuncPtr = reinterpret_cast<ArithmeticFuncPtr>(mulFuncAddr);
  ArithmeticFuncPtr divFuncPtr = reinterpret_cast<ArithmeticFuncPtr>(divFuncAddr);

  // Call the functions using the function pointers
  double x = 5.0;
  double y = 3.0;
  std::cout << "Add: " << addFuncPtr(x, y) << std::endl;
  std::cout << "Subtract: " << subFuncPtr(x, y) << std::endl;
  std::cout << "Multiply: " << mulFuncPtr(x, y) << std::endl;
  std::cout << "Divide: " << divFuncPtr(x, y) << std::endl;

  clock_t start = clock();
  for (int i = 0; i < 1000000000; i++) {
    addFuncPtr(x, y);
  }
  clock_t end = clock();
  double timeTaken = static_cast<double>(end - start) / CLOCKS_PER_SEC;
  std::cout << "Time taken for 10**9 iterations: " << timeTaken << " seconds, "
            << timeTaken / 1000000000.0 << "s / iter" << std::endl;

  delete EE;

  return 0;
}
