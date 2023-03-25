#include "llvm/IRReader/IRReader.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/OptimizationLevel.h>
#include <llvm/Support/ManagedStatic.h>
#include "FmiCheck.h"
using namespace llvm;

static cl::OptionCategory CallCounterCategory{"call counter options"};

static cl::list<std::string> InputModules(cl::Positional, cl::desc("<Input modules>"), cl::OneOrMore);

//
// Created by Kenneth Guldbrandt Lausdahl on 05/01/2023.
//
int main(int Argc, char **Argv) {
    // Hide all options apart from the ones specific to this tool
    cl::HideUnrelatedOptions(CallCounterCategory);

    cl::ParseCommandLineOptions(Argc, Argv,
                                "Counts the number of static function "
                                "calls in the input IR file\n");

    // Makes sure llvm_shutdown() is called (which cleans up LLVM objects)
    //  http://llvm.org/docs/ProgrammersManual.html#ending-execution-with-llvm-shutdown
    llvm_shutdown_obj SDO;

    LoopAnalysisManager LAM;
    ModuleAnalysisManager MAM;
    FunctionAnalysisManager FAM;
    CGSCCAnalysisManager CGAM;

    PassBuilder PB;

    // Register all basic analyses with the managers.
    PB.registerModuleAnalyses(MAM);
    PB.registerCGSCCAnalyses(CGAM);
    PB.registerFunctionAnalyses(FAM);
    PB.registerLoopAnalyses(LAM);
    PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

    ModulePassManager MPM;
    MPM.addPass(FMIC::FmiCheck(llvm::errs()));

    for (unsigned i = 0; i != InputModules.size(); ++i) {

        // Parse the IR file passed on the command line.
        errs() << InputModules[i] << "\n";

        SMDiagnostic Err;
        LLVMContext Ctx;
        std::unique_ptr<Module> M = parseIRFile(InputModules[i], Err, Ctx);

        if (!M) {
            errs() << "Error reading bitcode file: " << InputModules[i] << "\n";
            Err.print(Argv[0], errs());
            return -1;
        }

        // Finally, run the passes registered with MPM
        MPM.run(*M, MAM);
    }

    return 0;
}
