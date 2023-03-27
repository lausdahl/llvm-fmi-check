//=============================================================================
// FILE:
//    HelloWorld.cpp
//
// DESCRIPTION:
//    Visits all functions in a module, prints their names and the number of
//    arguments via stderr. Strictly speaking, this is an analysis pass (i.e.
//    the functions are not modified). However, in order to keep things simple
//    there's no 'print' method here (every analysis pass should implement it).
//
// USAGE:
//    1. Legacy PM
//      opt -enable-new-pm=0 -load libHelloWorld.dylib -legacy-hello-world -disable-output `\`
//        <input-llvm-file>
//    2. New PM
//      opt -load-pass-plugin=libHelloWorld.dylib -passes="hello-world" `\`
//        -disable-output <input-llvm-file>
//
//
// License: MIT
//=============================================================================
#include "FmiCheck.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Operator.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/MemorySSA.h"
#include "llvm/Demangle/Demangle.h"
#include <cstdlib>
#include <llvm/ADT/APInt.h>
#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/Analysis/InlineCost.h>
#include <llvm/IR/DiagnosticInfo.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Value.h>
#include <string>
#include <unordered_map>

using namespace llvm;

//-----------------------------------------------------------------------------
// HelloWorld implementation
//-----------------------------------------------------------------------------
// No need to expose the internals of the pass to the outside world - keep
// everything in an anonymous namespace.
namespace {

std::map<Value*, int> instanceMap;
int instanceId;

void dumpInstanceMap(){
    errs() << "==== Instance map ==== \n";
    for(std::map<Value*, int>::iterator it = instanceMap.begin(); it!=instanceMap.end(); it++){
        it->first->dump();
        errs() << " points to : " << it->second << "\n";
    }
}

void findGlobalAnnotations(Module &M) {
    // first add direct function annotations to function objects
    auto global_annos = M.getNamedGlobal("llvm.global.annotations");
    if (global_annos) {
        auto a = cast<ConstantArray>(global_annos->getOperand(0));
        for (int i=0; i<a->getNumOperands(); i++) {
            auto e = cast<ConstantStruct>(a->getOperand(i));
            if (auto fn = dyn_cast<Function>(e->getOperand(0))) {
                auto anno = (cast<ConstantDataArray>(e->getOperand(1)->getOperand(0)))->getAsCString();
                if (anno == "simulate") {
                    fn->addFnAttr(anno);
                }
            }
        }
    }
}

AllocaInst *getInstanceAlloc(Value *val) {
    if (auto inst = dyn_cast<AllocaInst>(val)) {
        return inst;
    } else if (auto inst = dyn_cast<LoadInst>(val)) {
        return getInstanceAlloc(inst->getPointerOperand());
    } else if (auto inst = dyn_cast<GEPOperator>(val)) {
        // if struct member then alloc inst of containing struct
        return getInstanceAlloc(inst->getPointerOperand());
    } else {
        return nullptr;
    }
}

bool isSameAllocInst(Value* val1, Value* val2, AAResults &AA) {
    // assume local stack alloc for each instance; and not re-assigned
    AllocaInst* a1 = getInstanceAlloc(val1);
    AllocaInst* a2 = getInstanceAlloc(val2);
    if (a1 && a2 && AA.isMustAlias(a1,a2)) {
        return true;
    }
    return false;
}

int getInstanceId(Value* val, AAResults &AA) {
    for(std::map<Value*, int>::iterator it = instanceMap.begin(); it!=instanceMap.end(); it++){
        if (AA.isMustAlias(val, it->first) || isSameAllocInst(val, it->first, AA)) {
           return it->second;
        }
    }
    instanceMap[val] = ++instanceId;
    return instanceId;
}

// This method implements what the pass does
void visitor(Module &M, ModuleAnalysisManager &MAM) {
    FunctionAnalysisManager &FAM =
        MAM.getResult<FunctionAnalysisManagerModuleProxy>(M).getManager();

    for (auto &F: M) {
        // disregard function builtins and declarations
        if(F.isIntrinsic() || F.isDeclaration()){
            continue;
        }

        // get the alias analysis results
        AAResults &AA = FAM.getResult<AAManager>(F);

        for (auto &BB: F) {
            for (auto &Ins: BB) {
                if (auto *inst = dyn_cast<CallBase>(&Ins)) {
                    if (inst->isIndirectCall()) {
                        if(LoadInst* linst = dyn_cast<LoadInst>(inst->getCalledOperand())) {
                            auto* pointOp = linst->getPointerOperand();
                            if (IntrinsicInst* iinst = dyn_cast<llvm::IntrinsicInst>(pointOp)) {
                                if (iinst->getIntrinsicID() == Intrinsic::ptr_annotation){
                                    auto carr = (ConstantArray *)iinst->getOperand(1);
                                    auto str = ((ConstantDataArray *)(carr)->getOperand(0))->getAsCString();
                                    if(LoadInst* linstp = dyn_cast<LoadInst>(inst->getOperand(0))) {
                                        auto instance = linstp->getPointerOperand();
                                        auto instance_id = getInstanceId(instance, AA);
                                        errs() << str << "(" << instance_id << ")" << "\n";
                                    } else {
                                        errs() << str << "\n";
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

} // namespace

PreservedAnalyses FMIC::FmiCheck::run(Module &M, ModuleAnalysisManager &MAM) {
    visitor(M, MAM);
    return PreservedAnalyses::none();
}

//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
/* llvm::PassPluginLibraryInfo getFmiCheckPluginInfo() { */
/*     return {LLVM_PLUGIN_API_VERSION, "FmiCheck", LLVM_VERSION_STRING, */
/*         [](PassBuilder &PB) { */
/*             PB.registerPipelineParsingCallback( */
/*                     [](StringRef Name, ModulePassManager &MPM, */
/*                        ArrayRef<PassBuilder::PipelineElement>) { */
/*                         if (Name == "fmi-check") { */
/*                             MPM.addPass(FMIC::FmiCheck(llvm::errs())); */
/*                             return true; */
/*                         } */
/*                         return false; */
/*                     }); */
/*         }}; */
/* } */

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize HelloWorld when added to the pass pipeline on the
// command line, i.e. via '-passes=hello-world'
/* extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo */
/* llvmGetPassPluginInfo() { */
/*     return getFmiCheckPluginInfo(); */
/* } */
//
//-----------------------------------------------------------------------------
// Legacy PM Registration
//-----------------------------------------------------------------------------
// The address of this variable is used to uniquely identify the pass. The
// actual value doesn't matter.
/* char LegacyFmiCheck::ID = 0; */

// This is the core interface for pass plugins. It guarantees that 'opt' will
// recognize LegacyHelloWorld when added to the pass pipeline on the command
// line, i.e.  via '--legacy-hello-world'
/* static RegisterPass<LegacyFmiCheck> */
/* X("legacy-fmi-check", "Fmi Check Pass", */
/*   true, // This pass doesn't modify the CFG => true */
/*   false // This pass is not a pure analysis pass => false */
/*   ); */
