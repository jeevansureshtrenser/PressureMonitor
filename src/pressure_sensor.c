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
#include "pressure_sensor.h"

//******************************* Local Types **********************************

//******************************* Local Constants ******************************

//******************************* Local Variables ******************************

//******************************* Local Functions ******************************
ERROR_TYPE ReadPressure(int *, PRESSURE_CONFIG *);

//******************************* ReadPressure ****************************
//Purpose : Read pressure value from sensor
//Inputs  : int *aiReadval - pointer to store read pressure value
//Outputs : pressure value through pointer
//Return  : ERROR_TYPE
//Notes   : read pressure value from sensor
//******************************************************************************
ERROR_TYPE ReadPressure(int *aiReadval, PRESSURE_CONFIG *pstPressureConfig)
{
    int max = pstPressureConfig->iUpperOperatingRange + CALIB_CONST;
    int min = pstPressureConfig->iLowerOperatingRange - CALIB_CONST;
    ERROR_TYPE eRetVal = NO_ERROR;

    *aiReadval = ((rand() % (max - min + 1)) + min);

    if(aiReadval == DEF_CLEAR)
    {
        eRetVal = ERROR_INVALID;
    }
    else
    { 
        /* No Process*/ 
    }
    return eRetVal;
}

/* end of file*/