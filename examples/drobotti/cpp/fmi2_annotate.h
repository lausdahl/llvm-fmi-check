/* -------------------------------------------------------------------------
 * fmi.h
 * Struct with the corresponding function pointers for FMI 2.0.
 * Copyright QTronic GmbH. All rights reserved.
 * -------------------------------------------------------------------------*/

#ifndef FMI_H
#define FMI_H

#ifdef _WIN32
	#include <windows.h>
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_MAC
        // Other kinds of Mac OS
		#include <dlfcn.h>
		#define HMODULE void*
    #else
        // Unsupported platform
    #endif
#elif __linux
	#include <limits.h>
	#include <dlfcn.h>
	#define MAX_PATH PATH_MAX
	#define HMODULE void*
#endif



#include "fmi2Functions.h"

//#include "XmlParserCApi.h"

#define FMIATTR(x) __attribute__ ((annotate(x)))

typedef struct {
    //ModelDescription* modelDescription;

    HMODULE dllHandle; // fmu.dll handle
    /***************************************************
    Common Functions
    ****************************************************/
    fmi2GetTypesPlatformTYPE         *getTypesPlatform FMIATTR("fmi2GetTypesPlatform");
    fmi2GetVersionTYPE               *getVersion FMIATTR("fmi2GetVersion");
    fmi2SetDebugLoggingTYPE          *setDebugLogging FMIATTR("fmi2SetDebugLogging");
    fmi2InstantiateTYPE              *instantiate FMIATTR("fmi2Instantiate");
    fmi2FreeInstanceTYPE             *freeInstance FMIATTR("fmi2FreeInstance");
    fmi2SetupExperimentTYPE          *setupExperiment FMIATTR("fmi2SetupExperiment");
    fmi2EnterInitializationModeTYPE  *enterInitializationMode FMIATTR("fmi2EnterInitializationMode");
    fmi2ExitInitializationModeTYPE   *exitInitializationMode FMIATTR("fmi2ExitInitializationMode");
    fmi2TerminateTYPE                *terminate FMIATTR("fmi2Terminate");
    fmi2ResetTYPE                    *reset FMIATTR("fmi2Reset");
    fmi2GetRealTYPE                  *getReal FMIATTR("fmi2GetReal");
    fmi2GetIntegerTYPE               *getInteger FMIATTR("fmi2GetInteger");
    fmi2GetBooleanTYPE               *getBoolean FMIATTR("fmi2GetBoolean");
    fmi2GetStringTYPE                *getString FMIATTR("fmi2GetString");
    fmi2SetRealTYPE                  *setReal FMIATTR("fmi2SetReal");
    fmi2SetIntegerTYPE               *setInteger FMIATTR("fmi2SetInteger");
    fmi2SetBooleanTYPE               *setBoolean FMIATTR("fmi2SetBoolean");
    fmi2SetStringTYPE                *setString FMIATTR("fmi2SetString");
    fmi2GetFMUstateTYPE              *getFMUstate FMIATTR("fmi2GetFMUstate");
    fmi2SetFMUstateTYPE              *setFMUstate FMIATTR("fmi2SetFMUstate");
    fmi2FreeFMUstateTYPE             *freeFMUstate FMIATTR("fmi2FreeFMUstate");
    fmi2SerializedFMUstateSizeTYPE   *serializedFMUstateSize FMIATTR("fmi2SerializedFMUstateSize");
    fmi2SerializeFMUstateTYPE        *serializeFMUstate FMIATTR("fmi2SerializeFMUstate");
    fmi2DeSerializeFMUstateTYPE      *deSerializeFMUstate FMIATTR("fmi2DeSerializeFMUstate");
    fmi2GetDirectionalDerivativeTYPE *getDirectionalDerivative FMIATTR("fmi2GetDirectionalDerivative");
    /***************************************************
    Functions for FMI2 for Co-Simulation
    ****************************************************/
    fmi2SetRealInputDerivativesTYPE  *setRealInputDerivatives FMIATTR("fmi2SetRealInputDerivatives");
    fmi2GetRealOutputDerivativesTYPE *getRealOutputDerivatives FMIATTR("fmi2GetRealOutputDerivatives");
    fmi2DoStepTYPE                   *doStep FMIATTR("fmi2DoStep");
    fmi2CancelStepTYPE               *cancelStep FMIATTR("fmi2CancelStep");
    fmi2GetStatusTYPE                *getStatus FMIATTR("fmi2GetStatus");
    fmi2GetRealStatusTYPE            *getRealStatus FMIATTR("fmi2GetRealStatus");
    fmi2GetIntegerStatusTYPE         *getIntegerStatus FMIATTR("fmi2GetIntegerStatus");
    fmi2GetBooleanStatusTYPE         *getBooleanStatus FMIATTR("fmi2GetBooleanStatus");
    fmi2GetStringStatusTYPE          *getStringStatus FMIATTR("fmi2GetStringStatus");

    fmi2GetMaxStepsizeTYPE 	     *getMaxStepsize FMIATTR("fmi2GetMaxStepsize");
    //INTO CPS specific
    /***************************************************
    Functions for FMI2 for Model Exchange
    ****************************************************/
    fmi2EnterEventModeTYPE                *enterEventMode FMIATTR("fmi2EnterEventMode");
    fmi2NewDiscreteStatesTYPE             *newDiscreteStates FMIATTR("fmi2NewDiscreteStates");
    fmi2EnterContinuousTimeModeTYPE       *enterContinuousTimeMode FMIATTR("fmi2EnterContinuous");
    fmi2CompletedIntegratorStepTYPE       *completedIntegratorStep FMIATTR("fmi2CompletedIntegratorStep");
    fmi2SetTimeTYPE                       *setTime FMIATTR("fmi2Set");
    fmi2SetContinuousStatesTYPE           *setContinuousStates FMIATTR("fmi2SetContinuousStates");
    fmi2GetDerivativesTYPE                *getDerivatives FMIATTR("fmi2GetDerivatives");
    fmi2GetEventIndicatorsTYPE            *getEventIndicators FMIATTR("fmi2GetEventIndicators");
    fmi2GetContinuousStatesTYPE           *getContinuousStates FMIATTR("fmi2GetContinuousStates");
    fmi2GetNominalsOfContinuousStatesTYPE *getNominalsOfContinuousStates FMIATTR("fmi2GetNominalsOfContinuousStates");
} FMU;

#endif // FMI_H

