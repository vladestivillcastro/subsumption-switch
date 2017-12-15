/**                                                                     
 *  /file wb_sensors_torsotemps.h 
 *                                                                      
 *  Created by Carl Lusty in 2014.                                      
 *  Copyright (c) 2014 Carl Lusty                                       
 *  All rights reserved.                                                
 */                                                                     
 
#ifndef _wb_sensors_torsotemps_h
#define _wb_sensors_torsotemps_h

#include <gu_util.h>

/**
 * Body sensors c struct
 */
struct wb_sensors_torsotemps
{
                PROPERTY(float, HeadYaw)
                PROPERTY(float, HeadPitch)
                PROPERTY(float, LShoulderPitch)
                PROPERTY(float, LShoulderRoll)
                PROPERTY(float, LElbowYaw)
                PROPERTY(float, LElbowRoll)
                PROPERTY(float, LHipYawPitch)
                PROPERTY(float, LHipRoll)
                PROPERTY(float, LHipPitch)
                PROPERTY(float, RHipRoll)
                PROPERTY(float, RHipPitch)
                PROPERTY(float, RShoulderPitch)
                PROPERTY(float, RShoulderRoll)
                PROPERTY(float, RElbowYaw)
                PROPERTY(float, RElbowRoll)
 
};

#endif //_wb_sensors_torsotemps_h
