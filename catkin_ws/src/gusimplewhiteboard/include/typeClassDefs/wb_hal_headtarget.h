/**                                                                     
 *  /file wb_hal_headtarget.h 
 *                                                                      
 *  Created by Carl Lusty in 2014.                                      
 *  Copyright (c) 2014 Carl Lusty                                       
 *  All rights reserved.                                                
 */                                                                     
 
#ifndef _wb_hal_headtarget_h
#define _wb_hal_headtarget_h

#include <gu_util.h>

#define YAW_LIMIT_POS 90
#define YAW_LIMIT_NEG -YAW_LIMIT_POS
#define PITCH_LIMIT_POS 28
#define PITCH_LIMIT_NEG -20

/**
 * head c struct
 */
struct wb_hal_headtarget
{
        //Angles are in degrees
        PROPERTY(float, target_pitchAngle) ///< not sure, look it up in the AL_Docs
        PROPERTY(float, target_yawAngle) ///< 
            
        //One of these values will be ignored base on the movement_type selected
        PROPERTY(int, target_movement_time) ///< usec
        BIT_PROPERTY(head_stopped) ///< Will be true if the head is not moving, will also stop the head if set on the WB
                
        BIT_PROPERTY(head_cmd_mask)
        unsigned pad: 30;

#ifdef __cplusplus
    wb_hal_headtarget(float target_pitchAngle = 0, float target_yawAngle = 0, int target_movement_time = 1000000, bool head_stopped = true)
 {
        set_target_pitchAngle(target_pitchAngle);
        set_target_yawAngle(target_yawAngle);
        set_target_movement_time(target_movement_time);
        set_head_stopped(head_stopped);
        set_head_cmd_mask(false);
}
#endif
};

#endif //_wb_hal_headtarget_h
