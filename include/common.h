/************************************************************
* File name     : common.h
* Description   : header file for common
* Author        : Jeevan Suresh
* License       : Copyright (c) 2025 Trenser 
                    All Rights Reserved
**************************************************************/
#ifndef COMMON_H_
#define COMMON_H_

//******************************* Include Files ******************************* 
#include <time.h>

/******************************Macro Definitions******************************/

#define SUCCESS 				    0
#define FAILURE 				    -1
#define DEFAULT_MIN_THRESHOLD		10
#define DEFAULT_MAX_THRESHOLD		70
#define DEFAULT_MIN_OPERATING_RANGE	2
#define DEFAULT_MAX_OPERATING_RANGE	105
#define DEFAULT_CHECK_TIME			2
#define DEFAULT_RETRY_TIME_OUT		100 // in milliseconds
#define MAX_RETRY_COUNT             10

#define SUCCESS                     0   // For success return
#define DEF_CLEAR                   0
#define DEF_SET                     1
#define MSG_SIZE                    64
#define CALIB_CONST                 5

#define ONE_MILLS_IN_SEC            1000

/******************************Global Variable declaration******************************/

typedef enum
{
	NO_ERROR,
	ERROR_INVALID,
	ERROR_THRESHOLD_MIN,
	ERROR_THRESHOLD_MAX
}ERROR_TYPE;


typedef enum
{
	NORMAL_STATE,
	ERROR_STATE,
	SAFE_STATE,
}STATE_TYPE;

typedef struct
{
	const char *CONFIGVERSION;
	int         iLowerThresholdRange;
	int         iUpperThresholdRange;
	int         iLowerOperatingRange;
	int         iUpperOperatingRange;
	long int    liChecktime;
    long int    liRetrytimeout;
	int         iMaxTryCount;

}PRESSURE_CONFIG;

extern STATE_TYPE g_eSystemState;
extern PRESSURE_CONFIG g_stPressureConfig;
extern clock_t g_lStart_reference;
/******************************FUnction declaration******************************/
extern void PrintConfig(PRESSURE_CONFIG *stPressureConfig);
extern void PrintMessage(ERROR_TYPE errortype_t, int iReadVal);
extern ERROR_TYPE ReadConfigParams(PRESSURE_CONFIG *pstPressureConfig);
extern ERROR_TYPE CheckPollTime(long int lCheckTime);
extern ERROR_TYPE CheckOperatingRange(PRESSURE_CONFIG *pstPressureConfig,int iReadVal);
extern ERROR_TYPE CheckThresholdRange(PRESSURE_CONFIG *pstPressureConfig,int iReadVal);
extern ERROR_TYPE ProcessData(PRESSURE_CONFIG *pstPressureConfig);
extern ERROR_TYPE FaultHandler(PRESSURE_CONFIG *pstPressureConfig);
extern ERROR_TYPE SafeProcessing(void);
#endif /* COMMON_H_ */