/************************************************************
* File name     : main.c
* Description   : This file is the entry point to the system
* Author        : Jeevan Suresh
* License       : Copyright (c) 2025 Trenser 
                    All Rights Reserved
**************************************************************/

/******************************Include Files******************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include "common.h"
#include "pressure_sensor.h"
#include <unistd.h>
/******************************Function Definitions******************************/
/************************************************************
* File name     : BackgroundProcess
* Description   : to handle the background processing based on system state
* argument      : void
* Return        : int
**************************************************************/
void BackgroundProcess(void)
{
    while(1)
    {
        switch(g_eSystemState)
        {
            case NORMAL_STATE:
                if(ProcessData(&g_stPressureConfig) != NO_ERROR)
                {
                    printf("Error in processing data, switching to ERROR_STATE\n");
                    g_eSystemState = ERROR_STATE;
                }
                else
                {
                    sleep(g_stPressureConfig.liChecktime); // converting to micro seconds
                }
                break;
            case ERROR_STATE:
                // Handle error state
                if(FaultHandler(&g_stPressureConfig) != NO_ERROR)
                {
                    printf("Error could not be resolved, switching to SAFE_STATE\n");
                    g_eSystemState = SAFE_STATE;
                }
                else
                {
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
/************************************************************
* File name     : main
* Description   : entry point of the application
* argument      : void
* Return        : int
**************************************************************/
int main(int argc, char *argv[])
{
    g_lStart_reference = clock();
    printf("Read the application\n");
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
    printf("start the application\n");
    BackgroundProcess();
    printf("Exiting the application\n");
    return SUCCESS;
}


/* end of file*/

