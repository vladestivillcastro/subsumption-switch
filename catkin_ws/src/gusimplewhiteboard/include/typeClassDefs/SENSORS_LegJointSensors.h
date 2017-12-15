/**                                                                     
 *  /file SENSORS_LegJointSensors.h
 *                                                                      
 *  Created by Carl Lusty in 2013.                                      
 *  Copyright (c) 2013 Carl Lusty                                       
 *  All rights reserved.                                                
 */                                                                     

                                                                        
#ifndef SENSORS_LegJointSensors_DEFINED
#define SENSORS_LegJointSensors_DEFINED

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>

#include "wb_sensors_legs.h" 

namespace guWhiteboard
{
        class SENSORS_LegJointSensors : public wb_sensors_legs
        {

        public:

#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
                /// string constructor (see from_string() below)
                SENSORS_LegJointSensors(const std::string &str) { from_string(str); }

                void from_string(const std::string &/*str*/)
                {
                        //NYI, Good luck
                }

                std::string description() const
                {
                        std::stringstream ss;
                        ss << LKneePitch() << " LK, " << LAnklePitch() << " AP, " << LAnkleRoll() << " AR, " << RKneePitch() << " rk, " << RAnklePitch() << " ap, " << RAnkleRoll() << " ar, " << LHipYawPitch() << " LHY, " << LHipRoll() << " LHR, " << LHipPitch() << " LHP, " << RHipYawPitch() << " rhy, " << RHipRoll() << " rhr, " << RHipPitch() << "rhp";
//                        ss << RAD2DEG(LKneePitch()) << " LK, " << RAD2DEG(LAnklePitch()) << " AP, " << RAD2DEG(LAnkleRoll()) << " AR, " << RAD2DEG(RKneePitch()) << " rk, " << RAD2DEG(RAnklePitch()) << " ap, " << RAD2DEG(RAnkleRoll()) << " ar, " << RAD2DEG(LHipYawPitch()) << " LHY, " << RAD2DEG(LHipRoll()) << " LHR, " << RAD2DEG(LHipPitch()) << " LHP, " << RAD2DEG(RHipYawPitch()) << " rhy, " << RAD2DEG(RHipRoll()) << " rhr, " << RAD2DEG(RHipPitch()) << "rhp";
                        return ss.str();
                }
#endif // WHITEBOARD_POSTER_STRING_CONVERSION
        };
}

#endif //SENSORS_LegJointSensors_DEFINED
