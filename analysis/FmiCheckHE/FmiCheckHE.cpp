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

// This method implements what the pass does
    void visitor(Module &M) {
        errs() << "Module: " << M.getName() << "\n";
        for (auto &Func: M) {
            for (auto &BB: Func) {
                for (auto &Ins: BB) {

                    if(auto *invoke = dyn_cast<InvokeInst>(&Ins)){
                        int i =0;
                    }

                    if (auto *gep = dyn_cast<GEPOperator>(&Ins)) {
                        /*
                         * <result> = getelementptr <ty>, ptr <ptrval>{, [inrange] <ty> <idx>}*
                         * <result> = getelementptr inbounds <ty>, ptr <ptrval>{, [inrange] <ty> <idx>}*
                         * <result> = getelementptr <ty>, <N x ptr> <ptrval>, [inrange] <vector index type> <idx>
                         *
                         *  %61 = load %class.Fmi2Comp*, %class.Fmi2Comp** %9, align 8
                         *  %62 = getelementptr inbounds %class.Fmi2Comp, %class.Fmi2Comp* %61, i32 0, i32 1
                         *  ty = %class.Fmi2Comp
                         *  ptr = %class.Fmi2Comp* %61
                         *  ptr index = i32 0
                         *  field index = i32 1
                         * */



                        if (auto tp = gep->getSourceElementType()) {
                            if (tp->isStructTy() && tp->getStructName().equals("struct.FMU")) {


                                //GEP has two operants lets consider f[0].field2 vs f[0].field1
                                // first is the index though the pointer i.e. [0]
                                // second is the field i.e. field2 and field1 assuming its declared as field1, field2 => 0 and 1
                                if (gep->getNumIndices() > 1) {

                                    auto instanceOp = gep->getOperand(gep->getNumIndices());
                                    if (ConstantInt *instanceCI = dyn_cast<ConstantInt>(instanceOp)) {
                                        if (instanceOp->getType()->getTypeID() == Type::TypeID::StructTyID) {
                                            errs() << instanceOp << "\n";
                                        }
//                                        instanceCI->getType()
//
                                        auto op = gep->getOperand(gep->getNumIndices());
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

                    // If this is a call instruction then CB will be not null.
                    auto *CB = dyn_cast<CallBase>(&Ins);
                    if (nullptr == CB) {
                        continue;
                    }

                    if (CB->isIndirectCall()) {
                        /* errs() << "Call: "<< CB->getOpcode() << " " << CB->getOpcodeName() << " " << CB->getNumOperands() << "\n"; */
                        for (auto op = CB->op_begin(); op != CB->op_end(); op++) {
                            /* errs() << "Operand: "<< *op << op->getOperandNo() << "\n"; */
                        }
                    }
                }
            }
        }
        /* errs() << "(llvm-tutor) Hello from: "<< F.getName() << "\n"; */
        /* errs() << "(llvm-tutor)   number of arguments: " << F.arg_size() << "\n"; */
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
