/**                                                                     
 *  /file wb_sensors_leds.h 
 *                                                                      
 *  Created by Carl Lusty in 2014.                                      
 *  Copyright (c) 2014 Carl Lusty                                       
 *  All rights reserved.                                                
 */                                                                     
 
#ifndef _wb_sensors_leds_h
#define _wb_sensors_leds_h

#include <gu_util.h>

#define _wb_sensors_leds_h_NUMBER_OF_LEDS 77 	//taken from the c++ namespace enum value: LEDs::NUMBER_OF_LEDS
#define _wb_sensors_leds_h_RightEar10 19 	//taken from the c++ namespace enum value: LEDs::RightEar10

/**
 * Body sensors c struct
 */
struct wb_sensors_leds
{
                ARRAY_PROPERTY(u_int8_t, ears, _wb_sensors_leds_h_RightEar10)
                ARRAY_PROPERTY(u_int8_t, leds, _wb_sensors_leds_h_NUMBER_OF_LEDS - _wb_sensors_leds_h_RightEar10)
};

#endif //_wb_sensors_leds_h
