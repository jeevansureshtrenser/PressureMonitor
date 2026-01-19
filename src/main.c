//******************************* PRESSURE MONITOR *****************************
//  Copyright (c) 2026 Trenser Technologies Pvt Ltd.
//  All Rights Reserved 
//******************************************************************************
// 
// File     : main.c
// Summary  : Main functions for Pressure Monitor Application
// Note     : This file is part of Pressure Monitor Application.
// Author   : Jeevan Suresh
// Date     : 2026-06-15
// 
//******************************************************************************
 
//******************************* Include Files ********************************
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include "common.h"
#include "pressure_sensor.h"
#include <unistd.h>

//******************************* Local Types **********************************

//******************************* Local Constants ******************************

//******************************* Local Variables ******************************

//******************************* Local Functions ******************************

//******************************* BackgroundProcess ****************************
//Purpose : the background process handling different states
//Inputs  : void
//Outputs : None
//Return  : void
//Notes   : Idle loop for background processing
//******************************************************************************
void BackgroundProcess(void)
{
    while(1)
    {
        switch(g_eSystemState)
        {
            case NORMAL_STATE:
                if(ProcessData(&g_stPressureConfig) != NO_ERROR)
                {
                    printf("\nError Processing, switching to ERROR_STATE\n");
                    g_eSystemState = ERROR_STATE;
                }
                else
                {
                    sleep(g_stPressureConfig.liChecktime);
                }
                break;
            case ERROR_STATE:
                // Handle error state
                if(FaultHandler(&g_stPressureConfig) != NO_ERROR)
                {
                    printf("\nError Fault Handling, switching to SAFE_STATE\n");
                    g_eSystemState = SAFE_STATE;
                }
                else
                {
                    printf("\nRead Value normal, switching to NORMAL_STATE\n");
                    g_eSystemState = NORMAL_STATE;
                }
                break;
            case SAFE_STATE:
                // Handle safe state
                SafeProcessing();
                break;
            default:
                printf("Unknown system state!\n");
                exit(FAILURE);
                break;
        }
        
    }
}

//******************************* main **********************************
//Purpose : To print the configuration parameters
//Inputs  : PRESSURE_CONFIG *stPressureConfig - pointer to pressure
//          configuration structure
//Outputs : None
//Return  : void
//Notes   : print the configuration parameters
//******************************************************************************
int main(int argc, char *argv[])
{
    // Initialize global variables
    g_lStart_reference = clock();
    if(g_lStart_reference == DEF_CLEAR)
    {
        printf("Error initializing time reference\n");
        return FAILURE;
    }
    else
    {
        /* No Process*/
    }

    // Read configuration parameters
    if(ReadConfigParams(&g_stPressureConfig) != NO_ERROR)
    {
        printf("Error in reading configuration parameters\n");
        return FAILURE;
    }
    else
    {
        PrintConfig(&g_stPressureConfig);
        g_eSystemState = NORMAL_STATE;
    }

    // Start background processing
    BackgroundProcess();
    printf("Exiting the application\n");
    return SUCCESS;
}

/* end of file*/