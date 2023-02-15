#ifndef LLVM_FMI_CALL_FMICALL_H
#define LLVM_FMI_CALL_FMICALL_H

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;
namespace FMIC {
    struct FmiCall
            : public llvm::PassInfoMixin<FmiCall> {
    public:
        explicit FmiCall(llvm::raw_ostream &OutS) : OS(OutS) {}


        llvm::PreservedAnalyses run(Module &F, ModuleAnalysisManager &);

        // Part of the official API:
        //  https://llvm.org/docs/WritingAnLLVMNewPMPass.html#required-passes
        static bool isRequired() { return true; }

    private:
        llvm::raw_ostream &OS;
    };
}
#endif //LLVM_FMI_CALL_FMICALL_H
