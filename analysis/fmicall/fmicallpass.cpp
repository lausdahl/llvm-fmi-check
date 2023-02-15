#include "fmicall.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Operator.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Demangle/Demangle.h"
#include <llvm/IR/Instructions.h>
#include <string>
#include <unordered_map>
#include <iostream>

using namespace llvm;

namespace {

std::map<Value*, Value*> storeMap;
std::map<Value*, Value*> loadMap;

void dumpStoreMap(){
	errs() << "==== Stores map ====\n";
	for(std::map<Value*, Value*>::iterator it = storeMap.begin(); it!=storeMap.end(); it++){
            errs() << "Value : ";
            it->first->dump();
            errs() << " points to : ";
            it->second->dump();
	}
}

void dumpLoadMap(){
	errs() << "==== Loads map ==== \n";
	for(std::map<Value*, Value*>::iterator it = loadMap.begin(); it!=loadMap.end(); it++){
            errs() << "Value : ";
            it->first->dump();
            errs() << " points to : ";
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
                    /* storeMap.insert(std::pair<Value*, Value*>(pointOp, valOp)); */
                }

                if(auto* inst = dyn_cast<LoadInst>(&Ins)){
                    Value* pointOp = inst->getPointerOperand()->stripPointerCasts();
                    Value* loadVal = dyn_cast<Value>(inst);
                    loadMap[loadVal] = pointOp;
                    /* loadMap.insert(std::pair<Value*, Value*>(loadVal, pointOp)); */
                }

                if (auto *inst = dyn_cast<GEPOperator>(&Ins)) {
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
                                            errs() << "getStructName: " << tp->getStructName() << " " << *op << " "
                                                << " on instance " << instanceCI->getSExtValue()
                                                << "\n";
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                // If this is a call instruction then CB will be not null.
                auto *CB = dyn_cast<CallBase>(&Ins);
                if (nullptr == CB) {
                    continue;
                }

                if (CB->isIndirectCall()) {
                    errs() << "\nIndirect Call: " << "\n";
                    CB->dump();
                    CB->getCalledOperand()->dump();
                    if(LoadInst* linst = dyn_cast<LoadInst>(CB->getCalledOperand())) {

                        Value* addr;
                        std::map<Value*, Value*>::iterator it1 = loadMap.find(linst);
                        if (it1 != loadMap.end()) {
                            errs() << "found load\n";
                            addr = it1->second;
                        }
                        std::map<Value*, Value*>::iterator it2 = storeMap.find(addr);
                        if (it2 != storeMap.end()) {
                            errs() << "found store\n";
                            if (Function *func = dyn_cast<Function>(it2->second)) {
                                errs() << "Indirect Function: " << func->getName() << "\n";
                            }
                        }
                    }

                    for (auto op = CB->op_begin(); op != CB->op_end(); op++) {
                        /* errs() << "Operand: "<< *op << op->getOperandNo() << "\n"; */
                    }
                }
            }
        }
    }
    /* dumpLoadMap(); */
    /* dumpStoreMap(); */
}

} // namespace

PreservedAnalyses FMIC::FmiCall::run(Module &M, ModuleAnalysisManager &) {
    visitor(M);
    return PreservedAnalyses::all();
}


//-----------------------------------------------------------------------------
// New PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getFmiCheckPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "FmiCall", LLVM_VERSION_STRING,
        [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                    [](StringRef Name, ModulePassManager &MPM,
                       ArrayRef<PassBuilder::PipelineElement>) {
                        if (Name == "fmi-call") {
                            MPM.addPass(FMIC::FmiCall(llvm::errs()));
                            return true;
                        }
                        return false;
                    });
        }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return getFmiCheckPluginInfo();
}
