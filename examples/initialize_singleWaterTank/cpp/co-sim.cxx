#include "co-sim.hxx"
#include <optional>
#include <cstdint>
#include <string>
#include "MaestroRunTimeException.h"
#include "BooleanLogic.h"
#include "SimFmi2.h"
#include "Logger.h"
#include "SimMath.h"
void simulate(const char* __runtimeConfigPath)
{
	int status = 0;
	Math math = load_Math();
	Logger logger = load_Logger();
	FMI2 tankcontroller = load_FMI2("{8c4e810f-3df3-4a00-8276-176fa3c9f000}", "src/test/resources/watertankcontroller-c.fmu");
	FMI2 SingleWatertank = load_FMI2("{cfc65592-9ece-4563-9705-1581b6e7071c}", "src/test/resources/singlewatertank-20sim.fmu");
	FMI2Component crtlInstance = tankcontroller->instantiate("crtlInstance", false, false);
	FMI2Component wtInstance = SingleWatertank->instantiate("wtInstance", false, false);
	double START_TIME = 10.0;
	double END_TIME = 10.0;
	int FMI_ERROR = 3;
	int FMI_FATAL = 4;
	BooleanLogic booleanLogic;
	{
		std::optional<MaestroRunTimeException> exceptionCache0;
		try
		{
			booleanLogic = load_BooleanLogic();
			if(booleanLogic == nullptr)
			{
				throw MaestroRunTimeException("Failed load of: booleanLogic");
			}

			double wtinstanceRealShare[1] = {};
			int crtlinstanceBoolShare[1] = {};
			int crtlinstanceBoolIo[4] = {};
			double crtlinstanceRealIo[4] = {};
			unsigned int crtlinstanceUintVref[4] = {};
			double wtinstanceRealIo[22] = {};
			unsigned int wtinstanceUintVref[22] = {};
			double fixed_end_time = 0.0;
			fixed_end_time = END_TIME;
			status = wtInstance->fmu->setupExperiment(wtInstance->comp, true, 0.1, START_TIME, true, END_TIME);
			if(status == FMI_ERROR || status == FMI_FATAL)
			{
				if(status == FMI_ERROR)
				{
					logger->log(4, "SetupExperiment failed on '%s' with status: FMI_ERROR", wtInstance);
				}

				if(status == FMI_FATAL)
				{
					logger->log(4, "SetupExperiment failed on '%s' with status: FMI_FATAL", wtInstance);
				}

				throw MaestroRunTimeException("Failed to 'setupExperiment' on 'wtInstance'");
			}

			status = crtlInstance->fmu->setupExperiment(crtlInstance->comp, true, 0.1, START_TIME, true, END_TIME);
			if(status == FMI_ERROR || status == FMI_FATAL)
			{
				if(status == FMI_ERROR)
				{
					logger->log(4, "SetupExperiment failed on '%s' with status: FMI_ERROR", crtlInstance);
				}

				if(status == FMI_FATAL)
				{
					logger->log(4, "SetupExperiment failed on '%s' with status: FMI_FATAL", crtlInstance);
				}

				throw MaestroRunTimeException("Failed to 'setupExperiment' on 'crtlInstance'");
			}

			wtinstanceUintVref[0] = 0;
			wtinstanceRealIo[0] = 9.0;
			status = wtInstance->fmu->setReal(wtInstance->comp, wtinstanceUintVref, 1, wtinstanceRealIo);
			if(status == FMI_ERROR || status == FMI_FATAL)
			{
				if(status == FMI_ERROR)
				{
					logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", wtInstance);
				}

				if(status == FMI_FATAL)
				{
					logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", wtInstance);
				}

				throw MaestroRunTimeException("Failed to 'setReal' on 'wtInstance'");
			}

			wtinstanceUintVref[0] = 1;
			wtinstanceRealIo[0] = 1.0;
			status = wtInstance->fmu->setReal(wtInstance->comp, wtinstanceUintVref, 1, wtinstanceRealIo);
			if(status == FMI_ERROR || status == FMI_FATAL)
			{
				if(status == FMI_ERROR)
				{
					logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", wtInstance);
				}

				if(status == FMI_FATAL)
				{
					logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", wtInstance);
				}

				throw MaestroRunTimeException("Failed to 'setReal' on 'wtInstance'");
			}

			wtinstanceUintVref[0] = 2;
			wtinstanceRealIo[0] = 1.0;
			status = wtInstance->fmu->setReal(wtInstance->comp, wtinstanceUintVref, 1, wtinstanceRealIo);
			if(status == FMI_ERROR || status == FMI_FATAL)
			{
				if(status == FMI_ERROR)
				{
					logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", wtInstance);
				}

				if(status == FMI_FATAL)
				{
					logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", wtInstance);
				}

				throw MaestroRunTimeException("Failed to 'setReal' on 'wtInstance'");
			}

			wtinstanceUintVref[0] = 3;
			wtinstanceRealIo[0] = 9.81;
			status = wtInstance->fmu->setReal(wtInstance->comp, wtinstanceUintVref, 1, wtinstanceRealIo);
			if(status == FMI_ERROR || status == FMI_FATAL)
			{
				if(status == FMI_ERROR)
				{
					logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", wtInstance);
				}

				if(status == FMI_FATAL)
				{
					logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", wtInstance);
				}

				throw MaestroRunTimeException("Failed to 'setReal' on 'wtInstance'");
			}

			wtinstanceUintVref[0] = 4;
			wtinstanceRealIo[0] = 1.0;
			status = wtInstance->fmu->setReal(wtInstance->comp, wtinstanceUintVref, 1, wtinstanceRealIo);
			if(status == FMI_ERROR || status == FMI_FATAL)
			{
				if(status == FMI_ERROR)
				{
					logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", wtInstance);
				}

				if(status == FMI_FATAL)
				{
					logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", wtInstance);
				}

				throw MaestroRunTimeException("Failed to 'setReal' on 'wtInstance'");
			}

			wtinstanceUintVref[0] = 5;
			wtinstanceRealIo[0] = 0.0;
			status = wtInstance->fmu->setReal(wtInstance->comp, wtinstanceUintVref, 1, wtinstanceRealIo);
			if(status == FMI_ERROR || status == FMI_FATAL)
			{
				if(status == FMI_ERROR)
				{
					logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", wtInstance);
				}

				if(status == FMI_FATAL)
				{
					logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", wtInstance);
				}

				throw MaestroRunTimeException("Failed to 'setReal' on 'wtInstance'");
			}

			wtinstanceUintVref[0] = 6;
			wtinstanceRealIo[0] = 0.0;
			status = wtInstance->fmu->setReal(wtInstance->comp, wtinstanceUintVref, 1, wtinstanceRealIo);
			if(status == FMI_ERROR || status == FMI_FATAL)
			{
				if(status == FMI_ERROR)
				{
					logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", wtInstance);
				}

				if(status == FMI_FATAL)
				{
					logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", wtInstance);
				}

				throw MaestroRunTimeException("Failed to 'setReal' on 'wtInstance'");
			}

			crtlinstanceUintVref[0] = 0;
			crtlinstanceRealIo[0] = 2.0;
			status = crtlInstance->fmu->setReal(crtlInstance->comp, crtlinstanceUintVref, 1, crtlinstanceRealIo);
			if(status == FMI_ERROR || status == FMI_FATAL)
			{
				if(status == FMI_ERROR)
				{
					logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", crtlInstance);
				}

				if(status == FMI_FATAL)
				{
					logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", crtlInstance);
				}

				throw MaestroRunTimeException("Failed to 'setReal' on 'crtlInstance'");
			}

			crtlinstanceUintVref[0] = 1;
			crtlinstanceRealIo[0] = 1.0;
			status = crtlInstance->fmu->setReal(crtlInstance->comp, crtlinstanceUintVref, 1, crtlinstanceRealIo);
			if(status == FMI_ERROR || status == FMI_FATAL)
			{
				if(status == FMI_ERROR)
				{
					logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", crtlInstance);
				}

				if(status == FMI_FATAL)
				{
					logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", crtlInstance);
				}

				throw MaestroRunTimeException("Failed to 'setReal' on 'crtlInstance'");
			}

			status = wtInstance->fmu->enterInitializationMode(wtInstance->comp);
			if(status == FMI_ERROR || status == FMI_FATAL)
			{
				if(status == FMI_ERROR)
				{
					logger->log(4, "EnterInitializationMode failed on '%s' with status: FMI_ERROR", wtInstance);
				}

				if(status == FMI_FATAL)
				{
					logger->log(4, "EnterInitializationMode failed on '%s' with status: FMI_FATAL", wtInstance);
				}

				throw MaestroRunTimeException("Failed to 'enterInitializationMode' on 'wtInstance'");
			}

			status = crtlInstance->fmu->enterInitializationMode(crtlInstance->comp);
			if(status == FMI_ERROR || status == FMI_FATAL)
			{
				if(status == FMI_ERROR)
				{
					logger->log(4, "EnterInitializationMode failed on '%s' with status: FMI_ERROR", crtlInstance);
				}

				if(status == FMI_FATAL)
				{
					logger->log(4, "EnterInitializationMode failed on '%s' with status: FMI_FATAL", crtlInstance);
				}

				throw MaestroRunTimeException("Failed to 'enterInitializationMode' on 'crtlInstance'");
			}

			int stabilisation_loop = 5;
			while(stabilisation_loop >= 0)
			{
				crtlinstanceUintVref[0] = 4;
				status = crtlInstance->fmu->getBoolean(crtlInstance->comp, crtlinstanceUintVref, 1, crtlinstanceBoolIo);
				if(status == FMI_ERROR || status == FMI_FATAL)
				{
					if(status == FMI_ERROR)
					{
						logger->log(4, "GetBoolean failed on '%s' with status: FMI_ERROR", crtlInstance);
					}

					if(status == FMI_FATAL)
					{
						logger->log(4, "GetBoolean failed on '%s' with status: FMI_FATAL", crtlInstance);
					}

					throw MaestroRunTimeException("Failed to 'getBoolean' on 'crtlInstance'");
				}

				crtlinstanceBoolShare[0] = crtlinstanceBoolIo[0];
				wtinstanceUintVref[0] = 16;
				if(crtlinstanceBoolShare[0])
				{
					wtinstanceRealIo[0] = 1.0;
				}
				else
				{
					wtinstanceRealIo[0] = 0.0;
				}

				status = wtInstance->fmu->setReal(wtInstance->comp, wtinstanceUintVref, 1, wtinstanceRealIo);
				if(status == FMI_ERROR || status == FMI_FATAL)
				{
					if(status == FMI_ERROR)
					{
						logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", wtInstance);
					}

					if(status == FMI_FATAL)
					{
						logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", wtInstance);
					}

					throw MaestroRunTimeException("Failed to 'setReal' on 'wtInstance'");
				}

				wtinstanceUintVref[0] = 17;
				status = wtInstance->fmu->getReal(wtInstance->comp, wtinstanceUintVref, 1, wtinstanceRealIo);
				if(status == FMI_ERROR || status == FMI_FATAL)
				{
					if(status == FMI_ERROR)
					{
						logger->log(4, "GetReal failed on '%s' with status: FMI_ERROR", wtInstance);
					}

					if(status == FMI_FATAL)
					{
						logger->log(4, "GetReal failed on '%s' with status: FMI_FATAL", wtInstance);
					}

					throw MaestroRunTimeException("Failed to 'getReal' on 'wtInstance'");
				}

				wtinstanceRealShare[0] = wtinstanceRealIo[0];
				crtlinstanceUintVref[0] = 3;
				crtlinstanceRealIo[0] = wtinstanceRealShare[0];
				status = crtlInstance->fmu->setReal(crtlInstance->comp, crtlinstanceUintVref, 1, crtlinstanceRealIo);
				if(status == FMI_ERROR || status == FMI_FATAL)
				{
					if(status == FMI_ERROR)
					{
						logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", crtlInstance);
					}

					if(status == FMI_FATAL)
					{
						logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", crtlInstance);
					}

					throw MaestroRunTimeException("Failed to 'setReal' on 'crtlInstance'");
				}

				stabilisation_loop = stabilisation_loop - 1;
			}

			status = wtInstance->fmu->exitInitializationMode(wtInstance->comp);
			if(status == FMI_ERROR || status == FMI_FATAL)
			{
				if(status == FMI_ERROR)
				{
					logger->log(4, "ExitInitializationMode failed on '%s' with status: FMI_ERROR", wtInstance);
				}

				if(status == FMI_FATAL)
				{
					logger->log(4, "ExitInitializationMode failed on '%s' with status: FMI_FATAL", wtInstance);
				}

				throw MaestroRunTimeException("Failed to 'exitInitializationMode' on 'wtInstance'");
			}

			status = crtlInstance->fmu->exitInitializationMode(crtlInstance->comp);
			if(status == FMI_ERROR || status == FMI_FATAL)
			{
				if(status == FMI_ERROR)
				{
					logger->log(4, "ExitInitializationMode failed on '%s' with status: FMI_ERROR", crtlInstance);
				}

				if(status == FMI_FATAL)
				{
					logger->log(4, "ExitInitializationMode failed on '%s' with status: FMI_FATAL", crtlInstance);
				}

				throw MaestroRunTimeException("Failed to 'exitInitializationMode' on 'crtlInstance'");
			}

		}

		catch(MaestroRunTimeException& e)
		{
			exceptionCache0=e;
		}
		if(booleanLogic != nullptr)
		{
			delete booleanLogic;
			booleanLogic = nullptr;
			booleanLogic = nullptr;
		}

		if (exceptionCache0.has_value())
		{ 
			throw exceptionCache0.value();
		}
	}
	delete tankcontroller;
	tankcontroller = nullptr;
	delete SingleWatertank;
	SingleWatertank = nullptr;
	delete math;
	math = nullptr;
	delete logger;
	logger = nullptr;
}
