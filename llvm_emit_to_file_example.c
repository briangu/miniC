#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <stdio.h>

// Helper function to create an arithmetic function
LLVMValueRef createArithmeticFunction(LLVMContextRef context, LLVMModuleRef module, const char *name, LLVMOpcode opcode) {
    LLVMTypeRef doubleType = LLVMDoubleTypeInContext(context);
    LLVMTypeRef funcParamTypes[] = {doubleType, doubleType};
    LLVMTypeRef funcType = LLVMFunctionType(doubleType, funcParamTypes, 2, 0);

    LLVMValueRef func = LLVMAddFunction(module, name, funcType);
    LLVMSetLinkage(func, LLVMExternalLinkage);

    LLVMBasicBlockRef entryBlock = LLVMAppendBasicBlockInContext(context, func, "entry");
    LLVMBuilderRef builder = LLVMCreateBuilderInContext(context);
    LLVMPositionBuilderAtEnd(builder, entryBlock);

    LLVMValueRef lhs = LLVMGetParam(func, 0);
    LLVMValueRef rhs = LLVMGetParam(func, 1);
    LLVMValueRef result = LLVMBuildBinOp(builder, opcode, lhs, rhs, "result");
    LLVMBuildRet(builder, result);

    LLVMDisposeBuilder(builder);
    return func;
}

int main() {
    // Initialize the native target.
    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMLinkInMCJIT();

    // Create a new LLVM context and module.
    LLVMContextRef context = LLVMContextCreate();
    LLVMModuleRef module = LLVMModuleCreateWithNameInContext("my_module", context);

    // Create arithmetic functions.
    createArithmeticFunction(context, module, "add", LLVMFAdd);
    createArithmeticFunction(context, module, "sub", LLVMFSub);
    createArithmeticFunction(context, module, "mul", LLVMFMul);
    createArithmeticFunction(context, module, "div", LLVMFDiv);

    // Get the default target triple and create the target machine.
    char *defaultTargetTriple = LLVMGetDefaultTargetTriple();
    LLVMSetTarget(module, defaultTargetTriple);

    char *errorMessage;
    LLVMTargetRef targetRef;
    if (LLVMGetTargetFromTriple(defaultTargetTriple, &targetRef, &errorMessage) != 0) {
        fprintf(stderr, "Error: %s\n", errorMessage);
        LLVMDisposeMessage(errorMessage);
        return 1;
    }

    LLVMTargetMachineRef targetMachine = LLVMCreateTargetMachine(targetRef, defaultTargetTriple, "", "", LLVMCodeGenLevelDefault, LLVMRelocDefault, LLVMCodeModelDefault);
    LLVMDisposeMessage(defaultTargetTriple);

    // Emit the object file.
    LLVMCodeGenFileType fileType = LLVMObjectFile;
    if (LLVMTargetMachineEmitToFile(targetMachine, module, "my_module.o", fileType, &errorMessage)) {
        fprintf(stderr, "Error: %s\n", errorMessage);
        LLVMDisposeMessage(errorMessage);
        return 1;
    }

    // Clean up.
    LLVMDisposeModule(module);
    LLVMDisposeTargetMachine(targetMachine);
    LLVMContextDispose(context);

    return 0;
}

