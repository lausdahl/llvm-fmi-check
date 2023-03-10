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
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/PassManager.h>
#include <string>
#include <unordered_map>

using namespace llvm;

//-----------------------------------------------------------------------------
// HelloWorld implementation
//-----------------------------------------------------------------------------
// No need to expose the internals of the pass to the outside world - keep
// everything in an anonymous namespace.
namespace {

std::string FMU[] = {
    "DUMMY",
    /***************************************************
            Common Functions
            ****************************************************/
    "getTypesPlatform",
    "getVersion",
    "setDebugLogging",
    "instantiate",
    "freeInstance",
    "setupExperiment",
    "enterInitializationMode",
    "exitInitializationMode",
    "terminate",
    "reset",
    "getReal",
    "getInteger",
    "getBoolean",
    "getString",
    "setReal",
    "setInteger",
    "setBoolean",
    "setString",
    "getFMUstate",
    "setFMUstate",
    "freeFMUstate",
    "serializedFMUstateSize",
    "serializeFMUstate",
    "deSerializeFMUstate",
    "getDirectionalDerivative",
    /***************************************************
            Functions for FMI2 for Co-Simulation
            ****************************************************/
    "setRealInputDerivatives",
    "getRealOutputDerivatives",
    "doStep",
    "cancelStep",
    "getStatus",
    "getRealStatus",
    "getIntegerStatus",
    "getBooleanStatus",
    "getStringStatus",

    //INTO CPS specific
    "getMaxStepsize",
    /***************************************************
            Functions for FMI2 for Model Exchange
            ****************************************************/
    "enterEventMode",
    "newDiscreteStates",
    "enterContinuousTimeMode",
    "completedIntegratorStep",
    "setTime",
    "setContinuousStates",
    "getDerivatives",
    "getEventIndicators",
    "getContinuousStates",
    "getNominalsOfContinuousStates",
};

std::map<Value*, Value*> storeMap;
std::map<Value*, Value*> loadMap;

void dumpStoreMap(){
    errs() << "==== Stores map ====\n";
    for(std::map<Value*, Value*>::iterator it = storeMap.begin(); it!=storeMap.end(); it++){
        errs() << "Value : " << it ->first;
        it->first->dump();
        errs() << " points to : " << it->second;
        it->second->dump();
    }
}

void dumpLoadMap(){
    errs() << "==== Loads map ==== \n";
    for(std::map<Value*, Value*>::iterator it = loadMap.begin(); it!=loadMap.end(); it++){
        errs() << "Value : " << it->first;
        it->first->dump();
        errs() << " points to : " << it->second;
        it->second->dump();
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

// This method implements what the pass does
void visitor(Module &M) {

    // iterate over functions in module - TBD fix order?
    for (auto &Func: M) {

        // disregard function builtins and declarations
        if(Func.isIntrinsic() || Func.isDeclaration()){
            continue;
        }

        /* if (demangle(Func.getName().str()) != "simulate(char const*)") { */

        /*     continue; */
        /* } */

        errs() << "Function: " << demangle(Func.getName().str()) << "\n";

        // iterate over basic blocks in function
        for (auto &BB: Func) {

            // iterate over instructions in basic block
            for (auto &Ins: BB) {

                if (auto *inst = dyn_cast<CallBase>(&Ins)) {
                    if (inst->isIndirectCall()) {
                        if(LoadInst* linst = dyn_cast<LoadInst>(inst->getCalledOperand())) {
                            auto* pointOp = linst->getPointerOperand();
                            if (IntrinsicInst* iinst = dyn_cast<llvm::IntrinsicInst>(pointOp)) {
                                if (iinst->getIntrinsicID() == Intrinsic::ptr_annotation){
                                    auto carr = (ConstantArray *)iinst->getOperand(1);
                                    auto str = ((ConstantDataArray *)(carr)->getOperand(0))->getAsCString();
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


// Legacy PM implementation
struct LegacyFmiCheck : public FunctionPass {
    static char ID;

    LegacyFmiCheck() : FunctionPass(ID) {}

    // Main entry point - the name conveys what unit of IR this is to be run on.
    bool runOnFunction(Function &F) override {
        /* visitor(F); */
        // Doesn't modify the input unit of IR, hence 'false'
        return false;
    }
};
} // namespace

PreservedAnalyses FMIC::FmiCheck::run(Module &M, ModuleAnalysisManager &) {
    visitor(M);
    return PreservedAnalyses::all();
}


//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getFmiCheckPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "FmiCheck", LLVM_VERSION_STRING,
        [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                    [](StringRef Name, ModulePassManager &MPM,
                       ArrayRef<PassBuilder::PipelineElement>) {
                        if (Name == "fmi-check") {
                            MPM.addPass(FMIC::FmiCheck(llvm::errs()));
                            return true;
                        }
                        return false;
                    });
        }};
}

// This is the core interface for pass plugins. It guarantees that 'opt' will
// be able to recognize HelloWorld when added to the pass pipeline on the
// command line, i.e. via '-passes=hello-world'
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return getFmiCheckPluginInfo();
}
//
//-----------------------------------------------------------------------------
// Legacy PM Registration
//-----------------------------------------------------------------------------
// The address of this variable is used to uniquely identify the pass. The
// actual value doesn't matter.
char LegacyFmiCheck::ID = 0;

// This is the core interface for pass plugins. It guarantees that 'opt' will
// recognize LegacyHelloWorld when added to the pass pipeline on the command
// line, i.e.  via '--legacy-hello-world'
static RegisterPass<LegacyFmiCheck>
X("legacy-fmi-check", "Fmi Check Pass",
  true, // This pass doesn't modify the CFG => true
  false // This pass is not a pure analysis pass => false
  );
