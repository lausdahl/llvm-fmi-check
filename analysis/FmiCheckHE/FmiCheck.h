//
// Created by Kenneth Guldbrandt Lausdahl on 05/01/2023.
//

#ifndef LLVM_FMI_CHECK_FMICHECK_H
#define LLVM_FMI_CHECK_FMICHECK_H

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;
namespace FMIC {
    struct FmiCheck
            : public llvm::PassInfoMixin<FmiCheck> {
    public:
        explicit FmiCheck(llvm::raw_ostream &OutS) : OS(OutS) {}


        llvm::PreservedAnalyses run(Module &F, ModuleAnalysisManager &MAM);

        // Part of the official API:
        //  https://llvm.org/docs/WritingAnLLVMNewPMPass.html#required-passes
        static bool isRequired() { return true; }

    private:
        llvm::raw_ostream &OS;
    };
}
#endif //LLVM_FMI_CHECK_FMICHECK_H
