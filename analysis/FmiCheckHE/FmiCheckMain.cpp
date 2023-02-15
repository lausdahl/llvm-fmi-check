#include "llvm/IRReader/IRReader.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include "FmiCheck.h"
using namespace llvm;

//===----------------------------------------------------------------------===//
// static - implementation
//===----------------------------------------------------------------------===//
static void countStaticCalls(Module &M) {
    // Create a module pass manager and add StaticCallCounterPrinter to it.
    ModulePassManager MPM;
    MPM.addPass(FMIC::FmiCheck(llvm::errs()));

    // Create an analysis manager and register StaticCallCounter with it.
    ModuleAnalysisManager MAM;
//    MAM.registerPass([&] { return StaticCallCounter(); });

    // Register all available module analysis passes defined in PassRegisty.def.
    // We only really need PassInstrumentationAnalysis (which is pulled by
    // default by PassBuilder), but to keep this concise, let PassBuilder do all
    // the _heavy-lifting_.
    PassBuilder PB;
    PB.registerModuleAnalyses(MAM);

    // Finally, run the passes registered with MPM
    MPM.run(M, MAM);
}

static cl::OptionCategory CallCounterCategory{"call counter options"};

/* static cl::opt<std::string> InputModule{cl::Positional, */
/*                                         cl::desc{"<Module to analyze>"}, */
/*                                         cl::value_desc{"bitcode filename"}, */
/*                                         cl::init(""), */
/*                                         cl::Required, */
/*                                         cl::cat{CallCounterCategory}}; */

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

    ModulePassManager MPM;
    MPM.addPass(FMIC::FmiCheck(llvm::errs()));

    // Create an analysis manager and register StaticCallCounter with it.
    ModuleAnalysisManager MAM;
//    MAM.registerPass([&] { return StaticCallCounter(); });

    // Register all available module analysis passes defined in PassRegisty.def.
    // We only really need PassInstrumentationAnalysis (which is pulled by
    // default by PassBuilder), but to keep this concise, let PassBuilder do all
    // the _heavy-lifting_.
    PassBuilder PB;
    PB.registerModuleAnalyses(MAM);

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

        // Run the analysis and print the results
        /* countStaticCalls(*M); */
    }

    return 0;
}
