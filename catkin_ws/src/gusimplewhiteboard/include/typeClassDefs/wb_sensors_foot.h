/**                                                                     
 *  /file wb_sensors_foot.h 
 *                                                                      
 *  Created by Carl Lusty in 2014.                                      
 *  Copyright (c) 2014 Carl Lusty                                       
 *  All rights reserved.                                                
 */                                                                     
 
#ifndef _wb_sensors_foot_h
#define _wb_sensors_foot_h

#include <gu_util.h>

/**
 * Body sensors c struct
 */
struct wb_sensors_foot
{
                PROPERTY(float, LFoot_FSR_FrontLeft)
                PROPERTY(float, LFoot_FSR_FrontRight)
                PROPERTY(float, LFoot_FSR_RearLeft)
                PROPERTY(float, LFoot_FSR_RearRight)
                PROPERTY(float, LFoot_FSR_CenterOfPressure_X)
                PROPERTY(float, LFoot_FSR_CenterOfPressure_Y)
                
                PROPERTY(float, RFoot_FSR_FrontLeft)
                PROPERTY(float, RFoot_FSR_FrontRight)
                PROPERTY(float, RFoot_FSR_RearLeft)
                PROPERTY(float, RFoot_FSR_RearRight)
                PROPERTY(float, RFoot_FSR_CenterOfPressure_X)
                PROPERTY(float, RFoot_FSR_CenterOfPressure_Y)
                
                PROPERTY(bool, LFoot_Bumper_Left)
                PROPERTY(bool, LFoot_Bumper_Right)
                PROPERTY(bool, RFoot_Bumper_Left)
                PROPERTY(bool, RFoot_Bumper_Right)
};

#endif //_wb_sensors_foot_h
