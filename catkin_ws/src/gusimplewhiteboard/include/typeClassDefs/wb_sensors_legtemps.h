/**                                                                     
 *  /file wb_sensors_legtemps.h 
 *                                                                      
 *  Created by Carl Lusty in 2014.                                      
 *  Copyright (c) 2014 Carl Lusty                                       
 *  All rights reserved.                                                
 */                                                                     
 
#ifndef _wb_sensors_legtemps_h
#define _wb_sensors_legtemps_h

#include <gu_util.h>

/**
 * Body sensors c struct
 */
struct wb_sensors_legtemps
{
                PROPERTY(float, LKneePitch)
                PROPERTY(float, LAnklePitch)
                PROPERTY(float, LAnkleRoll)
                PROPERTY(float, RKneePitch)
                PROPERTY(float, RAnklePitch)
                PROPERTY(float, RAnkleRoll)
 
};

#endif //_wb_sensors_legtemps_h
