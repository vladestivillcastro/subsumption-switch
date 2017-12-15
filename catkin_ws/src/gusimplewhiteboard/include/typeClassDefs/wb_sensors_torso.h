/**                                                                     
 *  /file wb_sensors_torso.h 
 *                                                                      
 *  Created by Carl Lusty in 2014.                                      
 *  Copyright (c) 2014 Carl Lusty                                       
 *  All rights reserved.                                                
 */                                                                     
 
#ifndef _wb_sensors_torso_h
#define _wb_sensors_torso_h

#include <gu_util.h>

/**
 * Body sensors c struct
 */
struct wb_sensors_torso
{
                PROPERTY(float, HeadYaw)  //RADIANS
                PROPERTY(float, HeadPitch) // RADIANS
                PROPERTY(float, LShoulderPitch)
                PROPERTY(float, LShoulderRoll)
                PROPERTY(float, LElbowYaw)
                PROPERTY(float, LElbowRoll)
                PROPERTY(float, RShoulderPitch)
                PROPERTY(float, RShoulderRoll)
                PROPERTY(float, RElbowYaw)
                PROPERTY(float, RElbowRoll)
};

#endif //_wb_sensors_torso_h
