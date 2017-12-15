/**                                                                     
 *  /file SENSORS_SonarSensors.h
 *                                                                      
 *  Created by Carl Lusty in 2013.                                      
 *  Copyright (c) 2013 Carl Lusty                                       
 *  All rights reserved.                                                
 */                                                                     

                                                                        
#ifndef SENSORS_SonarSensors_DEFINED
#define SENSORS_SonarSensors_DEFINED

#include "wb_sensors_sonar.h" 

namespace guWhiteboard                                                  
{
        namespace Sonar
        {
                typedef enum ReadingCodesEnum {
                        Left0,
                        Left1,
                        Left2,
                        Left3,
                        Left4,
                        Left5,
                        Left6,
                        Left7,
                        Left8,
                        Left9,
                        Center0,
                        Center1,
                        Center2,
                        Center3,
                        Center4,
                        Center5,
                        Center6,
                        Center7,
                        Center8,
                        Center9,
                        Right0,
                        Right1,
                        Right2,
                        Right3,
                        Right4,
                        Right5,
                        Right6,
                        Right7,
                        Right8,
                        Right9,
                        NUMBER_OF_READINGS
                } ReadingCode;
        }
        
        
        class SENSORS_SonarSensors : public wb_sensors_sonar
        {
                
        public:
                SENSORS_SonarSensors()
                {

                }
                
#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
                /// string constructor (see from_string() below)
                SENSORS_SonarSensors(const std::string &str) { from_string(str); }
                
                void from_string(const std::string &str)
                {
	                std::istringstream iss(str);
	                std::string token;
			for (int i=0; i< Sonar::NUMBER_OF_READINGS && getline(iss, token, ','); i++  )  
			{ u_int8_t value = static_cast<u_int8_t>(atoi(token.c_str())); 
					set_sonar(value,i); 
			}
                }
                
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wunreachable-code"
                
                /**
                 * return the head and body stiffness as a boolean
                 */
                std::string description() const
                {
                        std::stringstream ss;

                        for (int i = 0; i < (Sonar::NUMBER_OF_READINGS-1); i++)
                                ss << static_cast<int>(sonar(i)) << ",";
                        ss << static_cast<int>(sonar(Sonar::NUMBER_OF_READINGS-1)); //Don't want a ', ' for the last element.
                        
                        return ss.str();
                }
#endif // WHITEBOARD_POSTER_STRING_CONVERSION
                
        };
}

#endif //SENSORS_SonarSensors_DEFINED
