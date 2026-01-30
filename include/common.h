//****************************** PRESSURE MONITOR ******************************
//  Copyright (c) 2026 Trenser Technologies Pvt Ltd.
//  All Rights Reserved 
//******************************************************************************
// 
// Summary		: Common header file for Pressure Monitor Application
// Note    		: This file is part of Pressure Monitor Application.

//******************************************************************************

#ifndef COMMON_H_
#define COMMON_H_

//****************************** Include Files *********************************
#include <time.h>

//****************************** Global Types **********************************

//****************************** Global Constants ******************************
#define SUCCESS 				    0
#define FAILURE 				    -1

#define SUCCESS                     0   // For success return
#define DEF_CLEAR                   0
#define DEF_SET                     1
#define MSG_SIZE                    64

#define ONE_MILLS_IN_SEC            1000

#define OPTION_ONE                  '1'
#define OPTION_TWO                  '2'
#define NEWLINE_CHAR                '\n'
//****************************** Global Variables ******************************
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

//****************************** Forward declaration ***************************
extern void PrintConfig(PRESSURE_CONFIG *stPressureConfig);
extern void PrintMessage(ERROR_TYPE errortype_t, int iReadVal);
extern ERROR_TYPE ReadConfigParams(PRESSURE_CONFIG *pstPressureConfig);
extern ERROR_TYPE CheckPollTime(long int lCheckTime);
extern ERROR_TYPE CheckOperatingRange(PRESSURE_CONFIG *pstPressureConfig,
									int iReadVal);
extern ERROR_TYPE CheckThresholdRange( PRESSURE_CONFIG *pstPressureConfig,
									int iReadVal);
extern ERROR_TYPE ProcessData(PRESSURE_CONFIG *pstPressureConfig);
extern ERROR_TYPE FaultHandler(PRESSURE_CONFIG *pstPressureConfig);
extern ERROR_TYPE SafeProcessing(void);

//****************************** Inline Method Implementations *****************

#endif /* COMMON_H_ */