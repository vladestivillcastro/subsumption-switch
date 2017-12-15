/**                                                                     
 *  /file wb_sensors_legs.h 
 *                                                                      
 *  Created by Carl Lusty in 2014.                                      
 *  Copyright (c) 2014 Carl Lusty                                       
 *  All rights reserved.                                                
 */                                                                     
 
#ifndef _wb_sensors_legs_h
#define _wb_sensors_legs_h

#include <gu_util.h>

/**
 * Body sensors c struct
 */
struct wb_sensors_legs
{
                PROPERTY(float, LKneePitch)
                PROPERTY(float, LAnklePitch)
                PROPERTY(float, LAnkleRoll)
                PROPERTY(float, RKneePitch)
                PROPERTY(float, RAnklePitch)
                PROPERTY(float, RAnkleRoll)
                PROPERTY(float, LHipYawPitch)
                PROPERTY(float, LHipRoll)
                PROPERTY(float, LHipPitch)
                PROPERTY(float, RHipYawPitch)
                PROPERTY(float, RHipRoll)
                PROPERTY(float, RHipPitch)
};

#endif //_wb_sensors_legs_h
