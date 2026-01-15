/************************************************************
* File name     : processing.c
* Description   : source file for pressure sensor
* Author        : Jeevan Suresh
* License       : Copyright (c) 2025 Trenser 
                    All Rights Reserved
**************************************************************/
/******************************Include Files************************************/
#include <stdio.h>
#include <stdlib.h>
#include "pressure_sensor.h"

/******************************Function Declaration******************************/
ERROR_TYPE ReadPressure(int *);

/************************************************************
* Function name     : wswReadPrsure
* Description       : to return random value for pressure value
* Arguments         : void
* Return type       : int
**************************************************************/
ERROR_TYPE ReadPressure(int *afReadval)
{
    int max = DEFAULT_MAX_OPERATING_RANGE + CALIB_CONST;
    int min = DEFAULT_MIN_OPERATING_RANGE - CALIB_CONST;
    *afReadval = ((rand() % (max - min + 1)) + min);
    if(afReadval == DEF_CLEAR)
    {
        return ERROR_INVALID;
    }
    else
    { 
        /* No Process*/ 
    }
    return NO_ERROR;
}

/* end of file*/