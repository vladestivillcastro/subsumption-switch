/**                                                                     
 *  /file wb_sensors_sonar.h 
 *                                                                      
 *  Created by Carl Lusty in 2014.                                      
 *  Copyright (c) 2014 Carl Lusty                                       
 *  All rights reserved.                                                
 */                                                                     
 
#ifndef _wb_sensors_sonar_h
#define _wb_sensors_sonar_h

#include <gu_util.h>

#define _wb_sensors_leds_h_NUMBER_OF_READINGS 30 	//taken from the c++ namespace enum value: Sonar::NUMBER_OF_READINGS

/**
 * Body sensors c struct
 */
struct wb_sensors_sonar
{
                ARRAY_PROPERTY(u_int8_t, sonar, _wb_sensors_leds_h_NUMBER_OF_READINGS)
};

#endif //_wb_sensors_sonar_h
