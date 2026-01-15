/************************************************************
* File name     : common.c
* Description   : common file for project
* Author        : Jeevan Suresh
* License       : Copyright (c) 2025 Trenser 
                    All Rights Reserved
**************************************************************/
/**********************Include Files**************************/
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "common.h"
#include "pressure_sensor.h"

/*********************Global Variable Declaration***************/
PRESSURE_CONFIG g_stPressureConfig = {  "vr.01",
                                        DEFAULT_MIN_THRESHOLD,
                                        DEFAULT_MAX_THRESHOLD,
                                        DEFAULT_MIN_OPERATING_RANGE,
                                        DEFAULT_MAX_OPERATING_RANGE,
                                        DEFAULT_CHECK_TIME,
                                        DEFAULT_RETRY_TIME_OUT,
                                        MAX_RETRY_COUNT
                                    };

STATE_TYPE g_eSystemState = NORMAL_STATE;
clock_t g_lStart_reference = DEF_CLEAR;
//*********************Function Declarations********************/
void PrintConfig(PRESSURE_CONFIG *stPressureConfig);
void PrintMessage(ERROR_TYPE errortype_t, int iReadVal);
ERROR_TYPE ReadConfigParams(PRESSURE_CONFIG *pstPressureConfig);
ERROR_TYPE CheckPollTime(long int lCheckTime);
ERROR_TYPE CheckOperatingRange(PRESSURE_CONFIG *pstPressureConfig,int iReadVal);
ERROR_TYPE CheckThresholdRange(PRESSURE_CONFIG *pstPressureConfig,int iReadVal);
ERROR_TYPE ProcessData(PRESSURE_CONFIG *pstPressureConfig);
ERROR_TYPE FaultHandler(PRESSURE_CONFIG *pstPressureConfig);
ERROR_TYPE SafeProcessing(void);

//*********************Function Definitions*********************/
void PrintConfig(PRESSURE_CONFIG *stPressureConfig)
{
    printf("Configuration Parameters:\n");
    printf("CONFIGVERSION: %s\n", stPressureConfig->CONFIGVERSION);
    printf("Lower Threshold Range: %d\n", stPressureConfig->iLowerThresholdRange);
    printf("Upper Threshold Range: %d\n", stPressureConfig->iUpperThresholdRange);
    printf("Lower Operating Range: %d\n", stPressureConfig->iLowerOperatingRange);
    printf("Upper Operating Range: %d\n", stPressureConfig->iUpperOperatingRange);
    printf("Check Time: %ld\n", stPressureConfig->liChecktime);
    printf("Retry Timeout: %ld\n", stPressureConfig->liRetrytimeout);
    printf("Max Try Count: %d\n", stPressureConfig->iMaxTryCount);
}
/**********************************************************************
* Function name     : PrintMessage
* Description       : to print the message based on the parameter type
* Arguments         : WarningType warningType_t - to detect type of 
*                        warning, debug or invalid
*                     Parameter_type param_t - to detect the parameter 
*                        temperature or pressure
*                      int iReadVal - read data
* Return type       : void
*************************************************************************/
void PrintMessage(ERROR_TYPE errortype_t, int iReadVal)
{
    clock_t current_time                = DEF_CLEAR;
    double elapsed                      = DEF_CLEAR;
    clock_t current_time_sec            = DEF_CLEAR;
    clock_t current_milliseconds        = DEF_CLEAR;
    current_time = clock();
    if(current_time == DEF_CLEAR)
    {
        printf("| ERROR getting time info | ERROR   |\n");
        return;
    }
    elapsed = (double)(current_time - g_lStart_reference) / CLOCKS_PER_SEC;
    current_time_sec = (int)elapsed;
    current_milliseconds = (int)((elapsed - current_time_sec) * ONE_MILLS_IN_SEC);

    switch(errortype_t)
    {
        case NO_ERROR: 
                        printf("|  %6ld.%06ld | DEBUG   | %3d | NORMAL |\n",current_time_sec, current_milliseconds, iReadVal);
            break;
        case ERROR_INVALID:
                        printf("|  %6ld.%06ld | INVALID | %3d | Out of Bounds |\n",current_time_sec, current_milliseconds, iReadVal);
            break;
        case ERROR_THRESHOLD_MIN:
                        printf("|  %6ld.%06ld | WARNING | %3d | Below Min Threshold |\n", current_time_sec, current_milliseconds, iReadVal);
            break;
        case ERROR_THRESHOLD_MAX:
                        printf("|  %6ld.%06ld | WARNING | %3d | Above Max Threshold |\n", current_time_sec, current_milliseconds, iReadVal);
            break;
        default:
            break;
    }

}

/**********************************************************************
* Function name     : ReadConfigParams
* Description       : to read configuration parameters
* Arguments         : PRESSURE_CONFIG *pstPressureConfig - pointer to pressure configuration structure
*                      int iReadVal - read data
* Return type       : void
*************************************************************************/
ERROR_TYPE ReadConfigParams(PRESSURE_CONFIG *pstPressureConfig)
{
    pstPressureConfig = (PRESSURE_CONFIG *)malloc(sizeof(PRESSURE_CONFIG));
    if(pstPressureConfig == NULL)
    {
        return ERROR_INVALID;
    }
    pstPressureConfig->CONFIGVERSION        = "v1.0";
    pstPressureConfig->iLowerThresholdRange = DEFAULT_MIN_THRESHOLD;
    pstPressureConfig->iUpperThresholdRange = DEFAULT_MAX_THRESHOLD;
    pstPressureConfig->iLowerOperatingRange = DEFAULT_MIN_OPERATING_RANGE;
    pstPressureConfig->iUpperOperatingRange = DEFAULT_MAX_OPERATING_RANGE;
    pstPressureConfig->liChecktime          = DEFAULT_CHECK_TIME;
    pstPressureConfig->liRetrytimeout       = DEFAULT_RETRY_TIME_OUT;
    pstPressureConfig->iMaxTryCount         = MAX_RETRY_COUNT;

    return NO_ERROR;
}

/**********************************************************************
* Function name     : CheckPollTime
* Description       : to check the polling time
* Arguments         : long int lCheckTime - polling time
* Return type       : ERROR_TYPE
*************************************************************************/
ERROR_TYPE CheckPollTime(long int lCheckTime)    
{
    static long int lPreviousCheckTime = DEF_CLEAR;
    long int lCurrentTime = DEF_CLEAR;
    lCurrentTime = clock();

    if(lCurrentTime == DEF_CLEAR)
    {
        return ERROR_INVALID;
    }
    else
    {
        /* No Process*/
    }

    if(lPreviousCheckTime == DEF_CLEAR)
    {
        lPreviousCheckTime = lCurrentTime;
        return NO_ERROR;
    }
    else
    {
        /* No Process*/
    }
    if((lCurrentTime - lPreviousCheckTime) < lCheckTime)
    {
        return ERROR_INVALID;
    }
    lPreviousCheckTime = lCurrentTime;
    return NO_ERROR;
}

/**********************************************************************
* Function name     : CheckOperatingRange
* Description       : to check the operating range
* Arguments         : PRESSURE_CONFIG *pstPressureConfig - pointer to pressure configuration structure
*                     float fReadVal - read data
* Return type       : ERROR_TYPE
*************************************************************************/
ERROR_TYPE CheckOperatingRange(PRESSURE_CONFIG *pstPressureConfig,int iReadVal)
{
    if(iReadVal < pstPressureConfig->iLowerOperatingRange ||
       iReadVal > pstPressureConfig->iUpperOperatingRange)
    {
        return ERROR_INVALID;
    }
    else
    {
        /* No Process*/
    }
    return NO_ERROR;
}

/**********************************************************************
* Function name     : CheckThresholdRange
* Description       : to check the threshold range
* Arguments         : PRESSURE_CONFIG *pstPressureConfig - pointer to pressure configuration structure
*                     float fReadVal - read data
* Return type       : ERROR_TYPE
*************************************************************************/
ERROR_TYPE CheckThresholdRange(PRESSURE_CONFIG *pstPressureConfig,int iReadVal)
{
    if(iReadVal < pstPressureConfig->iLowerThresholdRange)
    {
        return ERROR_THRESHOLD_MIN;
    }
    else if(iReadVal > pstPressureConfig->iUpperThresholdRange)
    {
        return ERROR_THRESHOLD_MAX;
    }
    else
    {
        /* No Process*/
    }
    return NO_ERROR;
}

/**********************************************************************
* Function name     : ProcessData
* Description       : to process the data based on the parameter type
* Arguments         : PRESSURE_CONFIG *pstPressureConfig - pointer to pressure configuration structure
* Return type       : ERROR_TYPE
*************************************************************************/
ERROR_TYPE ProcessData(PRESSURE_CONFIG *pstPressureConfig)
{
    int iReadVal = DEF_CLEAR;
    ERROR_TYPE eRetVal = NO_ERROR;

    eRetVal = ReadPressure(&iReadVal);
    if(eRetVal != NO_ERROR)
    {
        return eRetVal;
    }
    else
    {
        /* No Process*/
    }

    if(CheckOperatingRange(pstPressureConfig, iReadVal) == NO_ERROR)
    {
        eRetVal = CheckThresholdRange(pstPressureConfig, iReadVal);
       if(eRetVal == NO_ERROR)
       {
           PrintMessage(NO_ERROR, iReadVal);
       }
       else
       {
           PrintMessage(eRetVal, iReadVal);
       }
    }
    else
    {
        return ERROR_INVALID;
    }

    return NO_ERROR;
}

/**********************************************************************
* Function name     : FaultHandler
* Description       : to handle the fault based on the parameter type
* Arguments         : PRESSURE_CONFIG *pstPressureConfig - pointer to pressure configuration structure
* Return type       : ERROR_TYPE
*************************************************************************/
ERROR_TYPE FaultHandler(PRESSURE_CONFIG *pstPressureConfig)
{
    int iTryCount = DEF_CLEAR;
    int iReadVal = DEF_CLEAR;
    ERROR_TYPE eRetVal = NO_ERROR;
    for(iTryCount = 0; iTryCount < pstPressureConfig->iMaxTryCount; iTryCount++)
    {

        eRetVal = ReadPressure(&iReadVal);
        if(eRetVal != NO_ERROR)
        {
            continue;
        }
        else
        {
            /* No Process*/
        }

        if(CheckOperatingRange(pstPressureConfig, iReadVal) == NO_ERROR)
        {
            printf("Fault Resolved in Attempt: %d\n", iTryCount + 1);
            return NO_ERROR;
        }
        else
        {
            continue;
        }
        printf("Retrying to read the pressure value, Attempt: %d\n", iTryCount + 1);
        usleep(pstPressureConfig->liRetrytimeout);
        
    }
    return ERROR_INVALID;
}

/**********************************************************************
* Function name     : SafeProcessing
* Description       : to print   the message based on the parameter type
* Arguments         : PRESSURE_CONFIG *pstPressureConfig - pointer to pressure configuration structure
*                     float fReadVal - read data
* Return type       : ERROR_TYPE
*************************************************************************/
ERROR_TYPE SafeProcessing(void)
{
    printf("System is in SAFE STATE. Taking necessary actions.\n");
    sleep(5); // Simulate safe processing delay
    return NO_ERROR;
}

/*end of file*/