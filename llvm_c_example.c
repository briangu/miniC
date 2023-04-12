#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Transforms/Scalar.h>
#include <llvm-c/Analysis.h> // Add this line
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  LLVMLinkInMCJIT();

  LLVMInitializeNativeTarget();
  LLVMInitializeNativeAsmPrinter();

  LLVMModuleRef mod = LLVMModuleCreateWithName("my_module");

  LLVMTypeRef param_types[] = {LLVMDoubleType(), LLVMDoubleType()};
  LLVMTypeRef ret_type = LLVMDoubleType();
  LLVMTypeRef func_type = LLVMFunctionType(ret_type, param_types, 2, 0);

  LLVMValueRef add = LLVMAddFunction(mod, "add", func_type);
  LLVMBasicBlockRef entry = LLVMAppendBasicBlock(add, "entry");

  LLVMBuilderRef builder = LLVMCreateBuilder();
  LLVMPositionBuilderAtEnd(builder, entry);

  LLVMValueRef a = LLVMGetParam(add, 0);
  LLVMValueRef b = LLVMGetParam(add, 1);
  LLVMValueRef res = LLVMBuildFAdd(builder, a, b, "res");
  LLVMBuildRet(builder, res);

  LLVMExecutionEngineRef engine;
  char *error = NULL;
  if (LLVMCreateExecutionEngineForModule(&engine, mod, &error)) {
    fprintf(stderr, "Failed to create execution engine: %s\n", error);
    LLVMDisposeMessage(error);
    exit(EXIT_FAILURE);
  }

  if (LLVMVerifyModule(mod, LLVMAbortProcessAction, &error)) {
    fprintf(stderr, "Error verifying module: %s\n", error);
    LLVMDisposeMessage(error);
    exit(EXIT_FAILURE);
  }

  LLVMDisposeBuilder(builder);

  typedef double (*FuncType)(double, double);
  FuncType add_func = (FuncType)LLVMGetFunctionAddress(engine, "add");

  double x = 5.0;
  double y = 3.0;
  printf("Add: %f\n", add_func(x, y));

  clock_t start = clock();
  for (long i = 0; i < 1000000000; ++i) {
    add_func(x, y);
  }
  clock_t end = clock();
  double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Time taken for 10**9 iterations: %f seconds, %g s/iter\n", time_taken, time_taken / 1000000000.0);

  LLVMDisposeExecutionEngine(engine);

  return 0;
}

