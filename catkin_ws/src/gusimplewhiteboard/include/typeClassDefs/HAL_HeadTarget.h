/**                                                                     
 *  /file HAL_HeadTarget.h
 *                                                                      
 *  Created by Carl Lusty in 2013.                                      
 *  Copyright (c) 2013 Carl Lusty                                       
 *  All rights reserved.                                                
 */                                                                     

                                                                        
#ifndef HAL_HeadTarget_DEFINED                                       
#define HAL_HeadTarget_DEFINED

#include "wb_hal_headtarget.h"


namespace guWhiteboard                                                  
{
        class HAL_HeadTarget : public wb_hal_headtarget
        {
        public:
            
                HAL_HeadTarget(): wb_hal_headtarget() {}
                
                void Stop()
                {
                        set_head_stopped(true);
                        set_head_cmd_mask(true);
                }

                void GoToWithTime(float yaw, float pitch, int time = 1000000)
                {
                    GoToWithTime_rad(static_cast<float>(DEG2RAD(yaw)), static_cast<float>(DEG2RAD(pitch), time));
                }
            
                void GoToWithTime_rad(float yaw, float pitch, int time = 1000000)
                {
/*                        if(yaw > float(DEG2RAD(YAW_LIMIT_POS)))
                                yaw = float(DEG2RAD(YAW_LIMIT_POS));
                        else if (yaw < float(DEG2RAD(YAW_LIMIT_NEG)))
                                yaw = float(DEG2RAD(YAW_LIMIT_NEG));
                        
                        if(pitch > float(DEG2RAD(PITCH_LIMIT_POS)))
                                pitch = float(DEG2RAD(PITCH_LIMIT_POS));
                        else if (pitch < float(DEG2RAD(PITCH_LIMIT_NEG)))
                                pitch = float(DEG2RAD(PITCH_LIMIT_NEG));
  */                      
                        set_target_pitchAngle(pitch);
                        set_target_yawAngle(yaw);
                        set_target_movement_time(time);
                        set_head_stopped(false);
                        set_head_cmd_mask(true);
                }
                
                
#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
                /// string constructor (see from_string() below)
                HAL_HeadTarget(const std::string &str) { from_string(str); }
                
                void from_string(const std::string &/*str*/)
                {
                       //NYI, Good luck
                }
                
                std::string description() const
                {
                        std::stringstream ss;
                        ss << target_pitchAngle() << " P, " << target_yawAngle() << " Y, " << target_movement_time() << " T, " << head_stopped() << " S, " << head_cmd_mask() << " M";
                        return ss.str();
                }
#endif // WHITEBOARD_POSTER_STRING_CONVERSION
        };
}

#endif //HAL_HeadTarget_DEFINED
