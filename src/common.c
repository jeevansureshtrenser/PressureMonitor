//******************************* PRESSURE MONITOR *****************************
//  Copyright (c) 2026 Trenser Technologies Pvt Ltd.
//  All Rights Reserved 
//******************************************************************************
// 
// File     : common.c
// Summary  : Common functions for Pressure Monitor Application
// Note     : This file is part of Pressure Monitor Application.
// Author   : Jeevan Suresh
// Date     : 2026-06-15
// 
//******************************************************************************
 
//******************************* Include Files ********************************
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "common.h"
#include "pressure_sensor.h"

//******************************* Local Types **********************************

//******************************* Local Constants ******************************

//******************************* Local Variables ******************************
PRESSURE_CONFIG g_stPressureConfig = {  
                                        "vr.01",
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

//******************************* Local Functions ******************************
void PrintConfig(PRESSURE_CONFIG *stPressureConfig);
void PrintMessage(ERROR_TYPE errortype_t, int iReadVal);
ERROR_TYPE ReadConfigParams(PRESSURE_CONFIG *pstPressureConfig);
ERROR_TYPE CheckPollTime(long int lCheckTime);
ERROR_TYPE CheckOperatingRange(PRESSURE_CONFIG *pstPressureConfig,int iReadVal);
ERROR_TYPE CheckThresholdRange(PRESSURE_CONFIG *pstPressureConfig,int iReadVal);
ERROR_TYPE ProcessData(PRESSURE_CONFIG *pstPressureConfig);
ERROR_TYPE FaultHandler(PRESSURE_CONFIG *pstPressureConfig);
ERROR_TYPE SafeProcessing(void);

//******************************* PrintConfig **********************************
//Purpose : To print the configuration parameters
//Inputs  : PRESSURE_CONFIG *stPressureConfig - pointer to pressure
//          configuration structure
//Outputs : None
//Return  : void
//Notes   : print the configuration parameters
//******************************************************************************
void PrintConfig(PRESSURE_CONFIG *stPressureConfig)
{
    if(stPressureConfig == NULL)
    {
        return;
    }
    else
    {
        /* No Process*/
    }
    printf("****************** CONFIGURATION PARAMETERS *******************\n");
    printf("Configuration Version   : %s\n", stPressureConfig->CONFIGVERSION);
    printf("Lower Threshold Range   : %d\n", stPressureConfig->iLowerThresholdRange);
    printf("Upper Threshold Range   : %d\n", stPressureConfig->iUpperThresholdRange);
    printf("Lower Operating Range   : %d\n", stPressureConfig->iLowerOperatingRange);
    printf("Upper Operating Range   : %d\n", stPressureConfig->iUpperOperatingRange);
    printf("Check Time              : %ld\n", stPressureConfig->liChecktime);
    printf("Retry Timeout           : %ld\n", stPressureConfig->liRetrytimeout);
    printf("Max Try Count           : %d\n", stPressureConfig->iMaxTryCount);
    printf("***************************************************************\n");
}

//******************************* PrintMessage *********************************
//Purpose : To print the message based on the parameter type
//Inputs  : ERROR_TYPE errortype_t - error type
//          int iReadVal - read data
//Outputs : None
//Return  : void
//Notes   : None
//******************************************************************************
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
    current_milliseconds = (int)(elapsed - current_time_sec);
    current_milliseconds = (int)current_milliseconds * ONE_MILLS_IN_SEC;

    switch(errortype_t)
    {
        case NO_ERROR: 
                        printf("|  %6ld.%06ld | DEBUG   | %3d PSI | NORMAL |\n",current_time_sec, current_milliseconds, iReadVal);
            break;
        case ERROR_INVALID:
                        printf("|  %6ld.%06ld | INVALID | %3d PSI | Out of Bounds |\n",current_time_sec, current_milliseconds, iReadVal);
            break;
        case ERROR_THRESHOLD_MIN:
                        printf("|  %6ld.%06ld | WARNING | %3d PSI | Below Min Threshold |\n", current_time_sec, current_milliseconds, iReadVal);
            break;
        case ERROR_THRESHOLD_MAX:
                        printf("|  %6ld.%06ld | WARNING | %3d PSI | Above Max Threshold |\n", current_time_sec, current_milliseconds, iReadVal);
            break;
        default:
            break;
    }

}

//******************************* ReadConfigParams *****************************
//Purpose : To read configuration parameters
//Inputs  : PRESSURE_CONFIG *pstPressureConfig - pointer to pressure
//          configuration structure
//Outputs : Pointer to pressure configuration structure is updated
//Return  : ERROR_TYPE
//Notes   : None
//******************************************************************************
ERROR_TYPE ReadConfigParams(PRESSURE_CONFIG *pstPressureConfig)
{
    ERROR_TYPE eRetVal = NO_ERROR;

    if(pstPressureConfig == NULL)
    {
        eRetVal = ERROR_INVALID;
    }
    else
    {
        /* No Process*/
    }
    pstPressureConfig->CONFIGVERSION        = "v1.0";
    pstPressureConfig->iLowerThresholdRange = DEFAULT_MIN_THRESHOLD;
    pstPressureConfig->iUpperThresholdRange = DEFAULT_MAX_THRESHOLD;
    pstPressureConfig->iLowerOperatingRange = DEFAULT_MIN_OPERATING_RANGE;
    pstPressureConfig->iUpperOperatingRange = DEFAULT_MAX_OPERATING_RANGE;
    pstPressureConfig->liChecktime          = DEFAULT_CHECK_TIME;
    pstPressureConfig->liRetrytimeout       = DEFAULT_RETRY_TIME_OUT;
    pstPressureConfig->iMaxTryCount         = MAX_RETRY_COUNT;

    return eRetVal;
}

//******************************* CheckPollTime ********************************
//Purpose : To check the polling time
//Inputs  : long int lCheckTime - polling time
//Outputs : None
//Return  : ERROR_TYPE
//Notes   : None
//******************************************************************************
ERROR_TYPE CheckPollTime(long int lCheckTime)    
{
    static long int lPreviousCheckTime  = DEF_CLEAR;
    long int lCurrentTime               = DEF_CLEAR;
    ERROR_TYPE eRetVal                  = NO_ERROR;

    lCurrentTime = clock();

    if(lCurrentTime == DEF_CLEAR)
    {
        eRetVal = ERROR_INVALID;
    }
    else
    {
        /* No Process*/
    }

    if(lPreviousCheckTime == DEF_CLEAR)
    {
        lPreviousCheckTime = lCurrentTime;
        eRetVal = NO_ERROR;
    }
    else
    {
        /* No Process*/
    }
    if((lCurrentTime - lPreviousCheckTime) < lCheckTime)
    {
        eRetVal = ERROR_INVALID;
    }
    lPreviousCheckTime = lCurrentTime;
    return eRetVal;
}

//******************************* CheckOperatingRange **************************
//Purpose : To check the operating range
//Inputs  : PRESSURE_CONFIG *pstPressureConfig - pointer to pressure
//          configuration structure
//          int iReadVal - read data
//Outputs : None
//Return  : ERROR_TYPE
//Notes   : None
//******************************************************************************
ERROR_TYPE CheckOperatingRange(PRESSURE_CONFIG *pstPressureConfig,int iReadVal)
{
    ERROR_TYPE eRetVal = NO_ERROR;

    if(iReadVal < pstPressureConfig->iLowerOperatingRange ||
       iReadVal > pstPressureConfig->iUpperOperatingRange)
    {
        eRetVal = ERROR_INVALID;
    }
    else
    {
        /* No Process*/
    }
    return eRetVal;
}

//******************************* CheckThresholdRange **************************
//Purpose : To check the threshold range
//Inputs  : PRESSURE_CONFIG *pstPressureConfig - pointer to pressure
//          configuration structure
//          int iReadVal - read data
//Outputs : None
//Return  : ERROR_TYPE
//Notes   : None
//******************************************************************************
ERROR_TYPE CheckThresholdRange(PRESSURE_CONFIG *pstPressureConfig,int iReadVal)
{
    ERROR_TYPE eRetVal = NO_ERROR;

    if(iReadVal < pstPressureConfig->iLowerThresholdRange)
    {
        eRetVal = ERROR_THRESHOLD_MIN;
    }
    else if(iReadVal > pstPressureConfig->iUpperThresholdRange)
    {
        eRetVal = ERROR_THRESHOLD_MAX;
    }
    else
    {
        /* No Process*/
    }
    return eRetVal;
}

//******************************* ProcessData **********************************
//Purpose : To process the data based on the parameter type
//Inputs  : PRESSURE_CONFIG *pstPressureConfig - pointer to pressure
//          configuration structure
//Outputs : None
//Return  : ERROR_TYPE
//Notes   : None
//******************************************************************************
ERROR_TYPE ProcessData(PRESSURE_CONFIG *pstPressureConfig)
{
    int iReadVal = DEF_CLEAR;
    ERROR_TYPE eRetVal = NO_ERROR;

    eRetVal = ReadPressure(&iReadVal, pstPressureConfig);
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
        PrintMessage(ERROR_INVALID, iReadVal);
        return ERROR_INVALID;
    }

    return NO_ERROR;
}

//******************************* FaultHandler *********************************
//Purpose : To handle the fault based on the parameter type
//Inputs  : PRESSURE_CONFIG *pstPressureConfig - pointer to pressure
//          configuration structure
//Outputs : None
//Return  : ERROR_TYPE
//Notes   : None
//******************************************************************************
ERROR_TYPE FaultHandler(PRESSURE_CONFIG *pstPressureConfig)
{
    int iTryCount = DEF_CLEAR;
    int iReadVal = DEF_CLEAR;
    ERROR_TYPE eRetVal = NO_ERROR;
    for(iTryCount = 0; iTryCount < pstPressureConfig->iMaxTryCount; iTryCount++)
    {

        eRetVal = ReadPressure(&iReadVal, pstPressureConfig);
        if(eRetVal != NO_ERROR)
        {
            PrintMessage(ERROR_INVALID, iReadVal);
        }
        else
        {
            /* No Process*/
        }

        if(CheckOperatingRange(pstPressureConfig, iReadVal) == NO_ERROR)
        {
            PrintMessage(NO_ERROR, iReadVal);
            eRetVal = NO_ERROR;
            break;
        }
        else
        {
            /* No Process*/
        }
        usleep(pstPressureConfig->liRetrytimeout);
        
    }
    return eRetVal;
}

//******************************* SafeProcessing *******************************
//Purpose : After reaching safe state, to handle the safe processing
//Inputs  : void
//Outputs : None
//Return  : ERROR_TYPE
//Notes   : None
//******************************************************************************
ERROR_TYPE SafeProcessing(void)
{
    printf("System is in SAFE STATE. Taking necessary actions.\n");
    printf("Waiting For User Intervention...\n");
    printf("Choose Option to Recover System:\n1. Reset System\n2. Shutdown System\n");
    char choice = getchar();
    if(choice == OPTION_ONE)
    {
        printf("System Reset Initiated...\n");
        g_eSystemState = NORMAL_STATE;
    }
    else if(choice == OPTION_TWO)
    {
        printf("System Shutdown Initiated...\n");
        exit(SUCCESS);
    }
    else if(choice == NEWLINE_CHAR)
    {
        // Ignore newline character
    }
    else
    {
        printf("Invalid Choice. Staying in SAFE STATE.\n");
    }

    usleep(ONE_MILLS_IN_SEC); // Simulate safe processing delay
    return NO_ERROR;
}

/*end of file*/