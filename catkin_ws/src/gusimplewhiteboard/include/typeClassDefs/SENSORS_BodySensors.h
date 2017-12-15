/**                                                                     
 *  /file SENSORS_BodySensors.h
 *                                                                      
 *  Created by Carl Lusty in 2013.                                      
 *  Copyright (c) 2013 Carl Lusty                                       
 *  All rights reserved.                                                
 */                                                                     

                                                                        
#ifndef SENSORS_BodySensors_DEFINED
#define SENSORS_BodySensors_DEFINED

#include "wb_sensors_body.h" 

namespace guWhiteboard                                                  
{
        class SENSORS_BodySensors : public wb_sensors_body
        {
        public:
#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
                /// string constructor (see from_string() below)
		SENSORS_BodySensors(const std::string &str) { from_string(str); }
                void from_string(const std::string &/*str*/) { /*NYI, Good luck*/ }

                std::string description() const
                {
                        std::stringstream ss;
                        ss << RAD2DEG(InertialSensor_AccX()) << " AccX, " << RAD2DEG(InertialSensor_AccY()) << " AccY, " << RAD2DEG(InertialSensor_AccZ()) << " AccZ, " << RAD2DEG(InertialSensor_GyrX()) << " GyrX, " << RAD2DEG(InertialSensor_GyrY()) << " GyrY, " << RAD2DEG(InertialSensor_GyrRef()) << " GyrRef, " << RAD2DEG(InertialSensor_AngleX()) << " X, " << RAD2DEG(InertialSensor_AngleY()) << " Y, " << ChestBoard_Button() << " Chest, " << Battery_Charge() << " Charge, " << Battery_Current() << " Current, " << US() << " US";
                        return ss.str();
                }
#endif // WHITEBOARD_POSTER_STRING_CONVERSION
        };
}

#endif //SENSORS_BodySensors_DEFINED
