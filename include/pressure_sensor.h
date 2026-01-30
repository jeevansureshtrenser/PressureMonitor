//****************************** PRESSURE MONITOR ******************************
//  Copyright (c) 2026 Trenser Technologies Pvt Ltd.
//  All Rights Reserved 
//******************************************************************************
// 
// Summary		: Header file for Pressure Monitor Application
// Note    		: This file is part of Pressure Monitor Application. Includes 
//                 pressure sensor related declarations.

//******************************************************************************
#ifndef __PRESSURE_SENSOR_H__
#define __PRESSURE_SENSOR_H__

//****************************** Include Files *********************************
#include "common.h"

//****************************** Global Types **********************************

//****************************** Global Constants ******************************
#define DEFAULT_MIN_THRESHOLD		10
#define DEFAULT_MAX_THRESHOLD		70
#define DEFAULT_MIN_OPERATING_RANGE	2
#define DEFAULT_MAX_OPERATING_RANGE	105
#define DEFAULT_CHECK_TIME			2
#define DEFAULT_RETRY_TIME_OUT		100 // in milliseconds
#define MAX_RETRY_COUNT             10
#define CALIB_CONST                 5

//****************************** Global Variables ******************************

//******************************* Forward Declaration***************************
extern ERROR_TYPE ReadPressure(int*, PRESSURE_CONFIG *);

#endif /*__PRESSURE_SENSOR_H__*/