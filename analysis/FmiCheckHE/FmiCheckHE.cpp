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
#include "llvm/Demangle/Demangle.h"
#include <cstdlib>
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
std::set<Value*> gepSet;

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

// This method implements what the pass does
void visitor(Module &M) {

    // iterate over functions in module - TBD fix order?
    for (auto &Func: M) {

        // disregard function builtins and declarations
        if(Func.isIntrinsic() || Func.isDeclaration()){
            continue;
        }

        errs() << "Function: " << demangle(Func.getName().str()) << "\n";

        // iterate over basic blocks in function
        for (auto &BB: Func) {

            // iterate over instructions in basic block
            for (auto &Ins: BB) {

                if(auto* inst = dyn_cast<StoreInst>(&Ins)){
                    Value* valOp = inst->getValueOperand()->stripPointerCasts();
                    Value* pointOp = inst->getPointerOperand()->stripPointerCasts();
                    storeMap[pointOp] = valOp;
                }

                if(auto* inst = dyn_cast<LoadInst>(&Ins)){
                    Value* pointOp = inst->getPointerOperand()->stripPointerCasts();
                    Value* loadVal = dyn_cast<Value>(inst);
                    loadMap[loadVal] = pointOp;
                }

                if (auto *inst = dyn_cast<GEPOperator>(&Ins)) {
                    Value *gepVal = dyn_cast<Value>(inst);
                    gepSet.insert(gepVal);

                    if (auto tp = inst->getSourceElementType()) {
                        if (tp->isStructTy() && tp->getStructName().equals("struct.FMU")) {
                            if (inst->getNumIndices() > 1) {
                                auto instanceOp = inst->getOperand(inst->getNumIndices());
                                if (ConstantInt *instanceCI = dyn_cast<ConstantInt>(instanceOp)) {
                                    if (instanceOp->getType()->getTypeID() == Type::TypeID::StructTyID) {
                                        errs() << instanceOp << "\n";
                                    }
                                    auto op = inst->getOperand(inst->getNumIndices());
                                    if (ConstantInt *CI = dyn_cast<ConstantInt>(op)) {
                                        if (CI->getBitWidth() <= 32) {
                                            int i = CI->getSExtValue();
                                            errs() << "getStructName: " << tp->getStructName() << " " << *op << " "
                                                << FMU[i] << " on instance " << instanceCI->getSExtValue()
                                                << "\n";
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if (auto *inst = dyn_cast<CallBase>(&Ins)) {
                    if (inst->isIndirectCall()) {
                        errs() << "Indirect Call: " << "\n";
                        inst->dump();
                        inst->getCalledOperand()->dump();

                        if(LoadInst* linst = dyn_cast<LoadInst>(inst->getCalledOperand())) {
                            std::map<Value*, Value*>::iterator it1 = loadMap.find(linst);
                            if (it1 != loadMap.end()) {
                                errs() << "found load " << it1->second->getName() << "\n";
                                Value* addr = it1->second;
                                addr->dump();

                                std::map<Value*, Value*>::iterator it2 = storeMap.find(addr);
                                if (it2 != storeMap.end()) {
                                    errs() << "found store\n";
                                    if (Function *func = dyn_cast<Function>(it2->second)) {
                                        errs() << "Indirect Function: " << func->getName() << "\n";
                                    }
                                }

                                std::set<Value*>::iterator it3 = gepSet.find(addr);
                                if (it3 != gepSet.end()) {
                                    errs() << "found gep\n";
                                    if (Function *func = dyn_cast<Function>(*it3)) {
                                        errs() << "Indirect Function: " << func->getName() << "\n";
                                    }
                                }
                            }
                        }
                        errs() << "\n";
                    }
                }
            }
        }
    }
    dumpLoadMap();
    dumpStoreMap();
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
