#include "co-sim.hxx"
#include <optional>
#include <cstdint>
#include <string>
#include "MaestroRunTimeException.h"
#include "BooleanLogic.h"
#include "DataWriter.h"
#include "DataWriterConfig.h"
#include "SimFmi2.h"
#include "Logger.h"
#include "SimMath.h"
void simulate(const char* __runtimeConfigPath)
{
	int status = 0;
	Math math = nullptr;
	Logger logger = nullptr;
	DataWriter dataWriter = nullptr;
	BooleanLogic booleanLogic = nullptr;

	FMI2 KIN = nullptr;

	FMI2 ACT = nullptr;

	FMI2 RBMQ = nullptr;

	FMI2Component RbmqFmu = nullptr;

	FMI2Component KinematicModel = nullptr;

	FMI2Component ActuationModel = nullptr;
	{
		std::optional<MaestroRunTimeException> exceptionCache0;
		try
		{
			math = load_Math();
			if(math == nullptr)
			{
				throw MaestroRunTimeException("math IS NULL ");
			}

			logger = load_Logger();
			if(logger == nullptr)
			{
				throw MaestroRunTimeException("logger IS NULL ");
			}

			dataWriter = load_DataWriter(__runtimeConfigPath);
			if(dataWriter == nullptr)
			{
				throw MaestroRunTimeException("dataWriter IS NULL ");
			}

			booleanLogic = load_BooleanLogic();
			if(booleanLogic == nullptr)
			{
				throw MaestroRunTimeException("booleanLogic IS NULL ");
			}

			KIN = load_FMI2("77236337-210e-4e9c-8f2c-c1a0677db21b", "file:/home/hejersbo/model_swap_example/fmus/unifmu_bicycle_kinematic/");
			if(KIN == nullptr)
			{
				throw MaestroRunTimeException("KIN IS NULL ");
			}

			ACT = load_FMI2("77236337-210e-4e9c-8f2c-c1a0677db21b", "file:/home/hejersbo/model_swap_example/fmus/unifmu_actuation_model/");
			if(ACT == nullptr)
			{
				throw MaestroRunTimeException("ACT IS NULL ");
			}

			RBMQ = load_FMI2("{cfc65592-ffff-4563-9705-1581b6e7071c}", "file:/home/hejersbo/model_swap_example/fmus/rbmq_local/");
			if(RBMQ == nullptr)
			{
				throw MaestroRunTimeException("RBMQ IS NULL ");
			}

			RbmqFmu = RBMQ->instantiate("RbmqFmu", false, false);
			if(RbmqFmu == nullptr)
			{
				throw MaestroRunTimeException("RbmqFmu IS NULL ");
			}

			KinematicModel = KIN->instantiate("KinematicModel", false, false);
			if(KinematicModel == nullptr)
			{
				throw MaestroRunTimeException("KinematicModel IS NULL ");
			}

			ActuationModel = ACT->instantiate("ActuationModel", false, false);
			if(ActuationModel == nullptr)
			{
				throw MaestroRunTimeException("ActuationModel IS NULL ");
			}

			double STEP_SIZE = 0.1;
			double START_TIME = 0.0;
			double END_TIME = 20.0;
			int FMI_OK = 0;
			int FMI_DISCARD = 2;
			int FMI_ERROR = 3;
			int FMI_FATAL = 4;
			BooleanLogic booleanLogic1;
			{
				std::optional<MaestroRunTimeException> exceptionCache1;
				try
				{
					booleanLogic1 = load_BooleanLogic();
					if(booleanLogic1 == nullptr)
					{
						throw MaestroRunTimeException("Failed load of: booleanLogic1");
					}

					double rbmqfmuRealShare[2] = {};
					double rbmqfmuRealIo[18] = {};
					double actuationmodelRealShare[2] = {};
					double actuationmodelRealIo[12] = {};
					unsigned int actuationmodelUintVref[12] = {};
					double kinematicmodelRealIo[16] = {};
					unsigned int kinematicmodelUintVref[16] = {};
					int rbmqfmuIntIo[18] = {};
					const char* rbmqfmuStringIo[18] = {};
					unsigned int rbmqfmuUintVref[18] = {};
					double fixed_end_time = 0.0;
					fixed_end_time = END_TIME;
					status = RbmqFmu->fmu->setupExperiment(RbmqFmu->comp, true, 0.1, START_TIME, true, END_TIME);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetupExperiment failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetupExperiment failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'setupExperiment' on 'RbmqFmu'");
					}

					status = KinematicModel->fmu->setupExperiment(KinematicModel->comp, true, 0.1, START_TIME, true, END_TIME);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetupExperiment failed on '%s' with status: FMI_ERROR", KinematicModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetupExperiment failed on '%s' with status: FMI_FATAL", KinematicModel);
						}

						throw MaestroRunTimeException("Failed to 'setupExperiment' on 'KinematicModel'");
					}

					status = ActuationModel->fmu->setupExperiment(ActuationModel->comp, true, 0.1, START_TIME, true, END_TIME);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetupExperiment failed on '%s' with status: FMI_ERROR", ActuationModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetupExperiment failed on '%s' with status: FMI_FATAL", ActuationModel);
						}

						throw MaestroRunTimeException("Failed to 'setupExperiment' on 'ActuationModel'");
					}

					rbmqfmuUintVref[0] = 0;
					rbmqfmuStringIo[0] = "localhost";
					status = RbmqFmu->fmu->setString(RbmqFmu->comp, rbmqfmuUintVref, 1, rbmqfmuStringIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetString failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetString failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'setString' on 'RbmqFmu'");
					}

					rbmqfmuUintVref[0] = 1;
					rbmqfmuIntIo[0] = 5672;
					status = RbmqFmu->fmu->setInteger(RbmqFmu->comp, rbmqfmuUintVref, 1, rbmqfmuIntIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetInteger failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetInteger failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'setInteger' on 'RbmqFmu'");
					}

					rbmqfmuUintVref[0] = 2;
					rbmqfmuStringIo[0] = "guest";
					status = RbmqFmu->fmu->setString(RbmqFmu->comp, rbmqfmuUintVref, 1, rbmqfmuStringIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetString failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetString failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'setString' on 'RbmqFmu'");
					}

					rbmqfmuUintVref[0] = 3;
					rbmqfmuStringIo[0] = "guest";
					status = RbmqFmu->fmu->setString(RbmqFmu->comp, rbmqfmuUintVref, 1, rbmqfmuStringIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetString failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetString failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'setString' on 'RbmqFmu'");
					}

					rbmqfmuUintVref[0] = 4;
					rbmqfmuStringIo[0] = "linefollower.data.to_cosim";
					status = RbmqFmu->fmu->setString(RbmqFmu->comp, rbmqfmuUintVref, 1, rbmqfmuStringIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetString failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetString failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'setString' on 'RbmqFmu'");
					}

					rbmqfmuUintVref[0] = 5;
					rbmqfmuIntIo[0] = 60;
					status = RbmqFmu->fmu->setInteger(RbmqFmu->comp, rbmqfmuUintVref, 1, rbmqfmuIntIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetInteger failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetInteger failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'setInteger' on 'RbmqFmu'");
					}

					rbmqfmuUintVref[0] = 6;
					rbmqfmuIntIo[0] = 10;
					status = RbmqFmu->fmu->setInteger(RbmqFmu->comp, rbmqfmuUintVref, 1, rbmqfmuIntIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetInteger failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetInteger failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'setInteger' on 'RbmqFmu'");
					}

					rbmqfmuUintVref[0] = 7;
					rbmqfmuIntIo[0] = 100;
					status = RbmqFmu->fmu->setInteger(RbmqFmu->comp, rbmqfmuUintVref, 1, rbmqfmuIntIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetInteger failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetInteger failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'setInteger' on 'RbmqFmu'");
					}

					rbmqfmuUintVref[0] = 8;
					rbmqfmuIntIo[0] = 1;
					status = RbmqFmu->fmu->setInteger(RbmqFmu->comp, rbmqfmuUintVref, 1, rbmqfmuIntIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetInteger failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetInteger failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'setInteger' on 'RbmqFmu'");
					}

					rbmqfmuUintVref[0] = 9;
					rbmqfmuStringIo[0] = "fmi_digital_twin_cd";
					status = RbmqFmu->fmu->setString(RbmqFmu->comp, rbmqfmuUintVref, 1, rbmqfmuStringIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetString failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetString failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'setString' on 'RbmqFmu'");
					}

					rbmqfmuUintVref[0] = 10;
					rbmqfmuStringIo[0] = "direct";
					status = RbmqFmu->fmu->setString(RbmqFmu->comp, rbmqfmuUintVref, 1, rbmqfmuStringIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetString failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetString failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'setString' on 'RbmqFmu'");
					}

					rbmqfmuUintVref[0] = 11;
					rbmqfmuStringIo[0] = "fmi_digital_twin_sh";
					status = RbmqFmu->fmu->setString(RbmqFmu->comp, rbmqfmuUintVref, 1, rbmqfmuStringIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetString failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetString failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'setString' on 'RbmqFmu'");
					}

					rbmqfmuUintVref[0] = 12;
					rbmqfmuStringIo[0] = "direct";
					status = RbmqFmu->fmu->setString(RbmqFmu->comp, rbmqfmuUintVref, 1, rbmqfmuStringIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetString failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetString failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'setString' on 'RbmqFmu'");
					}

					rbmqfmuUintVref[0] = 13;
					rbmqfmuStringIo[0] = "linefollower.data.from_cosim";
					status = RbmqFmu->fmu->setString(RbmqFmu->comp, rbmqfmuUintVref, 1, rbmqfmuStringIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetString failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetString failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'setString' on 'RbmqFmu'");
					}

					kinematicmodelUintVref[0] = 6;
					kinematicmodelRealIo[0] = 0.2;
					status = KinematicModel->fmu->setReal(KinematicModel->comp, kinematicmodelUintVref, 1, kinematicmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", KinematicModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", KinematicModel);
						}

						throw MaestroRunTimeException("Failed to 'setReal' on 'KinematicModel'");
					}

					kinematicmodelUintVref[0] = 7;
					kinematicmodelRealIo[0] = 0.2;
					status = KinematicModel->fmu->setReal(KinematicModel->comp, kinematicmodelUintVref, 1, kinematicmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", KinematicModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", KinematicModel);
						}

						throw MaestroRunTimeException("Failed to 'setReal' on 'KinematicModel'");
					}

					kinematicmodelUintVref[0] = 8;
					kinematicmodelRealIo[0] = 0.0;
					status = KinematicModel->fmu->setReal(KinematicModel->comp, kinematicmodelUintVref, 1, kinematicmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", KinematicModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", KinematicModel);
						}

						throw MaestroRunTimeException("Failed to 'setReal' on 'KinematicModel'");
					}

					kinematicmodelUintVref[0] = 9;
					kinematicmodelRealIo[0] = 0.0;
					status = KinematicModel->fmu->setReal(KinematicModel->comp, kinematicmodelUintVref, 1, kinematicmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", KinematicModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", KinematicModel);
						}

						throw MaestroRunTimeException("Failed to 'setReal' on 'KinematicModel'");
					}

					kinematicmodelUintVref[0] = 10;
					kinematicmodelRealIo[0] = 0.0;
					status = KinematicModel->fmu->setReal(KinematicModel->comp, kinematicmodelUintVref, 1, kinematicmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", KinematicModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", KinematicModel);
						}

						throw MaestroRunTimeException("Failed to 'setReal' on 'KinematicModel'");
					}

					kinematicmodelUintVref[0] = 11;
					kinematicmodelRealIo[0] = 0.0;
					status = KinematicModel->fmu->setReal(KinematicModel->comp, kinematicmodelUintVref, 1, kinematicmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", KinematicModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", KinematicModel);
						}

						throw MaestroRunTimeException("Failed to 'setReal' on 'KinematicModel'");
					}

					actuationmodelUintVref[0] = 0;
					actuationmodelRealIo[0] = 0.0;
					status = ActuationModel->fmu->setReal(ActuationModel->comp, actuationmodelUintVref, 1, actuationmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", ActuationModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", ActuationModel);
						}

						throw MaestroRunTimeException("Failed to 'setReal' on 'ActuationModel'");
					}

					actuationmodelUintVref[0] = 1;
					actuationmodelRealIo[0] = 0.84766;
					status = ActuationModel->fmu->setReal(ActuationModel->comp, actuationmodelUintVref, 1, actuationmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", ActuationModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", ActuationModel);
						}

						throw MaestroRunTimeException("Failed to 'setReal' on 'ActuationModel'");
					}

					actuationmodelUintVref[0] = 2;
					actuationmodelRealIo[0] = 0.834215;
					status = ActuationModel->fmu->setReal(ActuationModel->comp, actuationmodelUintVref, 1, actuationmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", ActuationModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", ActuationModel);
						}

						throw MaestroRunTimeException("Failed to 'setReal' on 'ActuationModel'");
					}

					actuationmodelUintVref[0] = 3;
					actuationmodelRealIo[0] = 0.99889;
					status = ActuationModel->fmu->setReal(ActuationModel->comp, actuationmodelUintVref, 1, actuationmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", ActuationModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", ActuationModel);
						}

						throw MaestroRunTimeException("Failed to 'setReal' on 'ActuationModel'");
					}

					actuationmodelUintVref[0] = 4;
					actuationmodelRealIo[0] = 0.0123;
					status = ActuationModel->fmu->setReal(ActuationModel->comp, actuationmodelUintVref, 1, actuationmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", ActuationModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", ActuationModel);
						}

						throw MaestroRunTimeException("Failed to 'setReal' on 'ActuationModel'");
					}

					actuationmodelUintVref[0] = 5;
					actuationmodelRealIo[0] = 0.05;
					status = ActuationModel->fmu->setReal(ActuationModel->comp, actuationmodelUintVref, 1, actuationmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", ActuationModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", ActuationModel);
						}

						throw MaestroRunTimeException("Failed to 'setReal' on 'ActuationModel'");
					}

					actuationmodelUintVref[0] = 11;
					actuationmodelRealIo[0] = 0.0;
					status = ActuationModel->fmu->setReal(ActuationModel->comp, actuationmodelUintVref, 1, actuationmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", ActuationModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", ActuationModel);
						}

						throw MaestroRunTimeException("Failed to 'setReal' on 'ActuationModel'");
					}

					status = RbmqFmu->fmu->enterInitializationMode(RbmqFmu->comp);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "EnterInitializationMode failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "EnterInitializationMode failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'enterInitializationMode' on 'RbmqFmu'");
					}

					status = KinematicModel->fmu->enterInitializationMode(KinematicModel->comp);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "EnterInitializationMode failed on '%s' with status: FMI_ERROR", KinematicModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "EnterInitializationMode failed on '%s' with status: FMI_FATAL", KinematicModel);
						}

						throw MaestroRunTimeException("Failed to 'enterInitializationMode' on 'KinematicModel'");
					}

					status = ActuationModel->fmu->enterInitializationMode(ActuationModel->comp);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "EnterInitializationMode failed on '%s' with status: FMI_ERROR", ActuationModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "EnterInitializationMode failed on '%s' with status: FMI_FATAL", ActuationModel);
						}

						throw MaestroRunTimeException("Failed to 'enterInitializationMode' on 'ActuationModel'");
					}

					actuationmodelUintVref[0] = 8;
					status = ActuationModel->fmu->getReal(ActuationModel->comp, actuationmodelUintVref, 1, actuationmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "GetReal failed on '%s' with status: FMI_ERROR", ActuationModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "GetReal failed on '%s' with status: FMI_FATAL", ActuationModel);
						}

						throw MaestroRunTimeException("Failed to 'getReal' on 'ActuationModel'");
					}

					actuationmodelRealShare[0] = actuationmodelRealIo[0];
					kinematicmodelUintVref[0] = 1;
					kinematicmodelRealIo[0] = actuationmodelRealShare[0];
					status = KinematicModel->fmu->setReal(KinematicModel->comp, kinematicmodelUintVref, 1, kinematicmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", KinematicModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", KinematicModel);
						}

						throw MaestroRunTimeException("Failed to 'setReal' on 'KinematicModel'");
					}

					actuationmodelUintVref[0] = 10;
					status = ActuationModel->fmu->getReal(ActuationModel->comp, actuationmodelUintVref, 1, actuationmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "GetReal failed on '%s' with status: FMI_ERROR", ActuationModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "GetReal failed on '%s' with status: FMI_FATAL", ActuationModel);
						}

						throw MaestroRunTimeException("Failed to 'getReal' on 'ActuationModel'");
					}

					actuationmodelRealShare[1] = actuationmodelRealIo[0];
					kinematicmodelUintVref[0] = 0;
					kinematicmodelRealIo[0] = actuationmodelRealShare[1];
					status = KinematicModel->fmu->setReal(KinematicModel->comp, kinematicmodelUintVref, 1, kinematicmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", KinematicModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", KinematicModel);
						}

						throw MaestroRunTimeException("Failed to 'setReal' on 'KinematicModel'");
					}

					rbmqfmuUintVref[0] = 101;
					status = RbmqFmu->fmu->getReal(RbmqFmu->comp, rbmqfmuUintVref, 1, rbmqfmuRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "GetReal failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "GetReal failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'getReal' on 'RbmqFmu'");
					}

					rbmqfmuRealShare[0] = rbmqfmuRealIo[0];
					actuationmodelUintVref[0] = 7;
					actuationmodelRealIo[0] = rbmqfmuRealShare[0];
					status = ActuationModel->fmu->setReal(ActuationModel->comp, actuationmodelUintVref, 1, actuationmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", ActuationModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", ActuationModel);
						}

						throw MaestroRunTimeException("Failed to 'setReal' on 'ActuationModel'");
					}

					rbmqfmuUintVref[0] = 102;
					status = RbmqFmu->fmu->getReal(RbmqFmu->comp, rbmqfmuUintVref, 1, rbmqfmuRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "GetReal failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "GetReal failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'getReal' on 'RbmqFmu'");
					}

					rbmqfmuRealShare[1] = rbmqfmuRealIo[0];
					actuationmodelUintVref[0] = 6;
					actuationmodelRealIo[0] = rbmqfmuRealShare[1];
					status = ActuationModel->fmu->setReal(ActuationModel->comp, actuationmodelUintVref, 1, actuationmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", ActuationModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", ActuationModel);
						}

						throw MaestroRunTimeException("Failed to 'setReal' on 'ActuationModel'");
					}

					status = RbmqFmu->fmu->exitInitializationMode(RbmqFmu->comp);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "ExitInitializationMode failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "ExitInitializationMode failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'exitInitializationMode' on 'RbmqFmu'");
					}

					status = KinematicModel->fmu->exitInitializationMode(KinematicModel->comp);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "ExitInitializationMode failed on '%s' with status: FMI_ERROR", KinematicModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "ExitInitializationMode failed on '%s' with status: FMI_FATAL", KinematicModel);
						}

						throw MaestroRunTimeException("Failed to 'exitInitializationMode' on 'KinematicModel'");
					}

					status = ActuationModel->fmu->exitInitializationMode(ActuationModel->comp);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "ExitInitializationMode failed on '%s' with status: FMI_ERROR", ActuationModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "ExitInitializationMode failed on '%s' with status: FMI_FATAL", ActuationModel);
						}

						throw MaestroRunTimeException("Failed to 'exitInitializationMode' on 'ActuationModel'");
					}

				}

				catch(MaestroRunTimeException& e)
				{
					exceptionCache1=e;
				}
				if(booleanLogic1 != nullptr)
				{
					delete booleanLogic1;
					booleanLogic1 = nullptr;
					booleanLogic1 = nullptr;
				}

				if (exceptionCache1.has_value())
				{ 
					throw exceptionCache1.value();
				}
			}
			BooleanLogic booleanLogic2;
			DataWriter dataWriter1;
			{
				std::optional<MaestroRunTimeException> exceptionCache2;
				try
				{
					dataWriter1 = load_DataWriter(__runtimeConfigPath);
					if(dataWriter1 == nullptr)
					{
						throw MaestroRunTimeException("Failed load of: dataWriter1");
					}

					booleanLogic2 = load_BooleanLogic();
					if(booleanLogic2 == nullptr)
					{
						throw MaestroRunTimeException("Failed load of: booleanLogic2");
					}

					int actuationmodelCurrentTimeFullStep = true;
					double actuationmodelCurrentTime = 0.0;
					int kinematicmodelCurrentTimeFullStep = true;
					double kinematicmodelCurrentTime = 0.0;
					int rbmqfmuCurrentTimeFullStep = true;
					double rbmqfmuCurrentTime = 0.0;
					double actuationmodelRealShare[3] = {};
					double rbmqfmuRealShare[2] = {};
					double kinematicmodelRealShare[4] = {};
					double actuationmodelRealIo[12] = {};
					unsigned int actuationmodelUintVref[12] = {};
					double kinematicmodelRealIo[16] = {};
					unsigned int kinematicmodelUintVref[16] = {};
					double rbmqfmuRealIo[18] = {};
					unsigned int rbmqfmuUintVref[18] = {};
					double jac_current_step_size = 0.0;
					double jac_step_size = 0.0;

					double jac_current_communication_point = 0.0;
					double jac_end_time = 0.0;
					jac_current_step_size = STEP_SIZE;
					jac_step_size = STEP_SIZE;
					jac_current_communication_point = START_TIME;
					jac_end_time = END_TIME;
					const char* datawriter_headers[9] = {"{RBMQ}.RbmqFmu.speed", "{RBMQ}.RbmqFmu.steering_angle", "{KIN}.KinematicModel.x", "{KIN}.KinematicModel.y", "{KIN}.KinematicModel.psi", "{KIN}.KinematicModel.v", "{ACT}.ActuationModel.act_angle", "{ACT}.ActuationModel.act_speed", "{ACT}.ActuationModel.act_accelleration"};
					DataWriterConfig datawriter_configuration = dataWriter1->writeHeader(9, datawriter_headers);
					rbmqfmuUintVref[0] = 101;
					rbmqfmuUintVref[1] = 102;
					status = RbmqFmu->fmu->getReal(RbmqFmu->comp, rbmqfmuUintVref, 2, rbmqfmuRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "GetReal failed on '%s' with status: FMI_ERROR", RbmqFmu);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "GetReal failed on '%s' with status: FMI_FATAL", RbmqFmu);
						}

						throw MaestroRunTimeException("Failed to 'getReal' on 'RbmqFmu'");
					}

					kinematicmodelUintVref[0] = 2;
					kinematicmodelUintVref[1] = 3;
					kinematicmodelUintVref[2] = 4;
					kinematicmodelUintVref[3] = 5;
					status = KinematicModel->fmu->getReal(KinematicModel->comp, kinematicmodelUintVref, 4, kinematicmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "GetReal failed on '%s' with status: FMI_ERROR", KinematicModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "GetReal failed on '%s' with status: FMI_FATAL", KinematicModel);
						}

						throw MaestroRunTimeException("Failed to 'getReal' on 'KinematicModel'");
					}

					actuationmodelUintVref[0] = 8;
					actuationmodelUintVref[1] = 9;
					actuationmodelUintVref[2] = 10;
					status = ActuationModel->fmu->getReal(ActuationModel->comp, actuationmodelUintVref, 3, actuationmodelRealIo);
					if(status == FMI_ERROR || status == FMI_FATAL)
					{
						if(status == FMI_ERROR)
						{
							logger->log(4, "GetReal failed on '%s' with status: FMI_ERROR", ActuationModel);
						}

						if(status == FMI_FATAL)
						{
							logger->log(4, "GetReal failed on '%s' with status: FMI_FATAL", ActuationModel);
						}

						throw MaestroRunTimeException("Failed to 'getReal' on 'ActuationModel'");
					}

					kinematicmodelRealShare[0] = kinematicmodelRealIo[0];
					kinematicmodelRealShare[1] = kinematicmodelRealIo[1];
					kinematicmodelRealShare[2] = kinematicmodelRealIo[2];
					kinematicmodelRealShare[3] = kinematicmodelRealIo[3];
					rbmqfmuRealShare[0] = rbmqfmuRealIo[0];
					rbmqfmuRealShare[1] = rbmqfmuRealIo[1];
					actuationmodelRealShare[0] = actuationmodelRealIo[0];
					actuationmodelRealShare[1] = actuationmodelRealIo[1];
					actuationmodelRealShare[2] = actuationmodelRealIo[2];
					double fmu_communicationpoints[3] = {};
					dataWriter1->writeDataPoint("rrrrrrrrr", datawriter_configuration, jac_current_communication_point, rbmqfmuRealShare[0], rbmqfmuRealShare[1], kinematicmodelRealShare[0], kinematicmodelRealShare[1], kinematicmodelRealShare[2], kinematicmodelRealShare[3], actuationmodelRealShare[0], actuationmodelRealShare[1], actuationmodelRealShare[2]);
					int any_discards = false;
					while(jac_current_communication_point + jac_current_step_size < jac_end_time)
					{

						kinematicmodelUintVref[0] = 0;
						kinematicmodelUintVref[1] = 1;
						kinematicmodelRealIo[0] = actuationmodelRealShare[2];
						kinematicmodelRealIo[1] = actuationmodelRealShare[0];
						status = KinematicModel->fmu->setReal(KinematicModel->comp, kinematicmodelUintVref, 2, kinematicmodelRealIo);
						if(status == FMI_ERROR || status == FMI_FATAL)
						{
							if(status == FMI_ERROR)
							{
								logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", KinematicModel);
							}

							if(status == FMI_FATAL)
							{
								logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", KinematicModel);
							}

							throw MaestroRunTimeException("Failed to 'setReal' on 'KinematicModel'");
						}

						actuationmodelUintVref[0] = 6;
						actuationmodelUintVref[1] = 7;
						actuationmodelRealIo[0] = rbmqfmuRealShare[1];
						actuationmodelRealIo[1] = rbmqfmuRealShare[0];
						status = ActuationModel->fmu->setReal(ActuationModel->comp, actuationmodelUintVref, 2, actuationmodelRealIo);
						if(status == FMI_ERROR || status == FMI_FATAL)
						{
							if(status == FMI_ERROR)
							{
								logger->log(4, "SetReal failed on '%s' with status: FMI_ERROR", ActuationModel);
							}

							if(status == FMI_FATAL)
							{
								logger->log(4, "SetReal failed on '%s' with status: FMI_FATAL", ActuationModel);
							}

							throw MaestroRunTimeException("Failed to 'setReal' on 'ActuationModel'");
						}

						any_discards = false;
						status = RbmqFmu->fmu->doStep(RbmqFmu->comp, jac_current_communication_point, jac_current_step_size, false);
						if(status == FMI_ERROR || status == FMI_FATAL)
						{
							if(status == FMI_ERROR)
							{
								logger->log(4, "DoStep failed on '%s' with status: FMI_ERROR", RbmqFmu);
							}

							if(status == FMI_FATAL)
							{
								logger->log(4, "DoStep failed on '%s' with status: FMI_FATAL", RbmqFmu);
							}

							throw MaestroRunTimeException("Failed to 'doStep' on 'RbmqFmu'");
						}

						if(status != FMI_OK)
						{
							if(status == FMI_DISCARD)
							{
								status = RbmqFmu->fmu->getRealStatus(RbmqFmu->comp, fmi2LastSuccessfulTime, &rbmqfmuCurrentTime);
								rbmqfmuCurrentTimeFullStep = false;
							}

						}
						else
						{
							rbmqfmuCurrentTime = jac_current_communication_point + jac_current_step_size;
							rbmqfmuCurrentTimeFullStep = true;
						}

						fmu_communicationpoints[0] = rbmqfmuCurrentTime;
						if(!rbmqfmuCurrentTimeFullStep)
						{
							logger->log(1, "## FMU: '%s' DISCARDED step at sim-time: %f for step-size: %f and proposed sim-time: %.15f", "RbmqFmu", jac_current_communication_point, jac_current_step_size, rbmqfmuCurrentTime);
							any_discards = any_discards || !rbmqfmuCurrentTimeFullStep;
						}

						status = KinematicModel->fmu->doStep(KinematicModel->comp, jac_current_communication_point, jac_current_step_size, false);
						if(status == FMI_ERROR || status == FMI_FATAL)
						{
							if(status == FMI_ERROR)
							{
								logger->log(4, "DoStep failed on '%s' with status: FMI_ERROR", KinematicModel);
							}

							if(status == FMI_FATAL)
							{
								logger->log(4, "DoStep failed on '%s' with status: FMI_FATAL", KinematicModel);
							}

							throw MaestroRunTimeException("Failed to 'doStep' on 'KinematicModel'");
						}

						if(status != FMI_OK)
						{
							if(status == FMI_DISCARD)
							{
								status = KinematicModel->fmu->getRealStatus(KinematicModel->comp, fmi2LastSuccessfulTime, &kinematicmodelCurrentTime);
								kinematicmodelCurrentTimeFullStep = false;
							}

						}
						else
						{
							kinematicmodelCurrentTime = jac_current_communication_point + jac_current_step_size;
							kinematicmodelCurrentTimeFullStep = true;
						}

						fmu_communicationpoints[1] = kinematicmodelCurrentTime;
						if(!kinematicmodelCurrentTimeFullStep)
						{
							logger->log(1, "## FMU: '%s' DISCARDED step at sim-time: %f for step-size: %f and proposed sim-time: %.15f", "KinematicModel", jac_current_communication_point, jac_current_step_size, kinematicmodelCurrentTime);
							any_discards = any_discards || !kinematicmodelCurrentTimeFullStep;
						}

						status = ActuationModel->fmu->doStep(ActuationModel->comp, jac_current_communication_point, jac_current_step_size, false);
						if(status == FMI_ERROR || status == FMI_FATAL)
						{
							if(status == FMI_ERROR)
							{
								logger->log(4, "DoStep failed on '%s' with status: FMI_ERROR", ActuationModel);
							}

							if(status == FMI_FATAL)
							{
								logger->log(4, "DoStep failed on '%s' with status: FMI_FATAL", ActuationModel);
							}

							throw MaestroRunTimeException("Failed to 'doStep' on 'ActuationModel'");
						}

						if(status != FMI_OK)
						{
							if(status == FMI_DISCARD)
							{
								status = ActuationModel->fmu->getRealStatus(ActuationModel->comp, fmi2LastSuccessfulTime, &actuationmodelCurrentTime);
								actuationmodelCurrentTimeFullStep = false;
							}

						}
						else
						{
							actuationmodelCurrentTime = jac_current_communication_point + jac_current_step_size;
							actuationmodelCurrentTimeFullStep = true;
						}

						fmu_communicationpoints[2] = actuationmodelCurrentTime;
						if(!actuationmodelCurrentTimeFullStep)
						{
							logger->log(1, "## FMU: '%s' DISCARDED step at sim-time: %f for step-size: %f and proposed sim-time: %.15f", "ActuationModel", jac_current_communication_point, jac_current_step_size, actuationmodelCurrentTime);
							any_discards = any_discards || !actuationmodelCurrentTimeFullStep;
						}

						kinematicmodelUintVref[0] = 2;
						kinematicmodelUintVref[1] = 3;
						kinematicmodelUintVref[2] = 4;
						kinematicmodelUintVref[3] = 5;
						status = KinematicModel->fmu->getReal(KinematicModel->comp, kinematicmodelUintVref, 4, kinematicmodelRealIo);
						if(status == FMI_ERROR || status == FMI_FATAL)
						{
							if(status == FMI_ERROR)
							{
								logger->log(4, "GetReal failed on '%s' with status: FMI_ERROR", KinematicModel);
							}

							if(status == FMI_FATAL)
							{
								logger->log(4, "GetReal failed on '%s' with status: FMI_FATAL", KinematicModel);
							}

							throw MaestroRunTimeException("Failed to 'getReal' on 'KinematicModel'");
						}

						rbmqfmuUintVref[0] = 101;
						rbmqfmuUintVref[1] = 102;
						status = RbmqFmu->fmu->getReal(RbmqFmu->comp, rbmqfmuUintVref, 2, rbmqfmuRealIo);
						if(status == FMI_ERROR || status == FMI_FATAL)
						{
							if(status == FMI_ERROR)
							{
								logger->log(4, "GetReal failed on '%s' with status: FMI_ERROR", RbmqFmu);
							}

							if(status == FMI_FATAL)
							{
								logger->log(4, "GetReal failed on '%s' with status: FMI_FATAL", RbmqFmu);
							}

							throw MaestroRunTimeException("Failed to 'getReal' on 'RbmqFmu'");
						}

						actuationmodelUintVref[0] = 8;
						actuationmodelUintVref[1] = 9;
						actuationmodelUintVref[2] = 10;
						status = ActuationModel->fmu->getReal(ActuationModel->comp, actuationmodelUintVref, 3, actuationmodelRealIo);
						if(status == FMI_ERROR || status == FMI_FATAL)
						{
							if(status == FMI_ERROR)
							{
								logger->log(4, "GetReal failed on '%s' with status: FMI_ERROR", ActuationModel);
							}

							if(status == FMI_FATAL)
							{
								logger->log(4, "GetReal failed on '%s' with status: FMI_FATAL", ActuationModel);
							}

							throw MaestroRunTimeException("Failed to 'getReal' on 'ActuationModel'");
						}

						kinematicmodelRealShare[0] = kinematicmodelRealIo[0];
						kinematicmodelRealShare[1] = kinematicmodelRealIo[1];
						kinematicmodelRealShare[2] = kinematicmodelRealIo[2];
						kinematicmodelRealShare[3] = kinematicmodelRealIo[3];
						rbmqfmuRealShare[0] = rbmqfmuRealIo[0];
						rbmqfmuRealShare[1] = rbmqfmuRealIo[1];
						actuationmodelRealShare[0] = actuationmodelRealIo[0];
						actuationmodelRealShare[1] = actuationmodelRealIo[1];
						actuationmodelRealShare[2] = actuationmodelRealIo[2];
						if(!any_discards)
						{
							jac_current_communication_point = jac_current_communication_point + jac_current_step_size;
							dataWriter1->writeDataPoint("rrrrrrrrr", datawriter_configuration, jac_current_communication_point, rbmqfmuRealShare[0], rbmqfmuRealShare[1], kinematicmodelRealShare[0], kinematicmodelRealShare[1], kinematicmodelRealShare[2], kinematicmodelRealShare[3], actuationmodelRealShare[0], actuationmodelRealShare[1], actuationmodelRealShare[2]);
							jac_current_step_size = jac_step_size;
						}

					}

					dataWriter1->close();
				}

				catch(MaestroRunTimeException& e)
				{
					exceptionCache2=e;
				}
				if(dataWriter1 != nullptr)
				{
					delete dataWriter1;
					dataWriter1 = nullptr;
					dataWriter1 = nullptr;
				}
				if(booleanLogic2 != nullptr)
				{
					delete booleanLogic2;
					booleanLogic2 = nullptr;
					booleanLogic2 = nullptr;
				}

				if (exceptionCache2.has_value())
				{ 
					throw exceptionCache2.value();
				}
			}
			RbmqFmu->fmu->terminate(RbmqFmu->comp);
			KinematicModel->fmu->terminate(KinematicModel->comp);
			ActuationModel->fmu->terminate(ActuationModel->comp);
		}

		catch(MaestroRunTimeException& e)
		{
			exceptionCache0=e;
		}
		if(RbmqFmu != nullptr)
		{
			RBMQ->freeInstance(RbmqFmu->comp);
			RbmqFmu = nullptr;
		}
		if(KinematicModel != nullptr)
		{
			KIN->freeInstance(KinematicModel->comp);
			KinematicModel = nullptr;
		}
		if(ActuationModel != nullptr)
		{
			ACT->freeInstance(ActuationModel->comp);
			ActuationModel = nullptr;
		}
		if(KIN != nullptr)
		{
			delete KIN;
			KIN = nullptr;
			KIN = nullptr;
		}
		if(ACT != nullptr)
		{
			delete ACT;
			ACT = nullptr;
			ACT = nullptr;
		}
		if(RBMQ != nullptr)
		{
			delete RBMQ;
			RBMQ = nullptr;
			RBMQ = nullptr;
		}
		if(math != nullptr)
		{
			delete math;
			math = nullptr;
			math = nullptr;
		}
		if(logger != nullptr)
		{
			delete logger;
			logger = nullptr;
			logger = nullptr;
		}
		if(dataWriter != nullptr)
		{
			delete dataWriter;
			dataWriter = nullptr;
			dataWriter = nullptr;
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
}
