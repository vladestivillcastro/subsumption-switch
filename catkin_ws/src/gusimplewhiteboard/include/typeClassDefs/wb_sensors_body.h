/**                                                                     
 *  /file wb_sensors_body.h 
 *                                                                      
 *  Created by Carl Lusty in 2014.                                      
 *  Copyright (c) 2014 Carl Lusty                                       
 *  All rights reserved.                                                
 */                                                                     
 
#ifndef _wb_sensors_body_h
#define _wb_sensors_body_h

#include <gu_util.h>

/**
 * Body sensors c struct
 */
struct wb_sensors_body
{
    PROPERTY(float, InertialSensor_AccX)
    PROPERTY(float, InertialSensor_AccY)
    PROPERTY(float, InertialSensor_AccZ)
    PROPERTY(float, InertialSensor_GyrX)
    PROPERTY(float, InertialSensor_GyrY)
    PROPERTY(float, InertialSensor_GyrRef)
    PROPERTY(float, InertialSensor_AngleX)
    PROPERTY(float, InertialSensor_AngleY)
                
    PROPERTY(float, Battery_Charge)         //check if this is a float or not
    PROPERTY(float, Battery_Current)        //check if this is a float or not
    PROPERTY(float, US)                     //Not sure about this type, might only be 1 byte

    PROPERTY(bool, ChestBoard_Button)       //could use a 1 bit var here
    bool pad1;
    int16_t pad2;
};

#endif //_wb_sensors_body_h
