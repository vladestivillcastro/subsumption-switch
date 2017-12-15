/**                                                                     
 *  /file wb_walk_controlstatus.h 
 *                                                                      
 *  Created by Carl Lusty in 2014.                                      
 *  Copyright (c) 2014 Carl Lusty                                       
 *  All rights reserved.                                                
 */                                                                     
 
#ifndef _wb_walk_controlstatus_h
#define _wb_walk_controlstatus_h

#include <gu_util.h>

#ifdef __cplusplus
#include <float.h>
#endif

#ifndef USE_UNSW_ODOMETRY
struct Odometry                         ///< needs to mimic UNSW odometry!
{
        float forward;
        float left;
        float turn;

#ifdef __cplusplus
   inline bool operator== (const Odometry& a) {
      return (	fabs(forward - a.forward) < FLT_EPSILON &&
		fabs(left - a.left) < FLT_EPSILON &&
		fabs(turn - a.turn) < FLT_EPSILON);
   }
#endif

};
#endif

/**
 * command for ControlStatus
 */
enum WALK_ControlStatus_Mode
{
        WALK_Disconnected,      ///< walk is disconnected
        WALK_Stop,              ///< walk stopped / stop walk
        WALK_Ready,             ///< walk ready
        WALK_Run,               ///< walking
        WALK_Ignore             ///< only interested in odomentry
};

/**
 * walk c struct
 */
struct wb_walk_controlstatus
{
	PROPERTY(float, forward)
        PROPERTY(float, left)
        PROPERTY(float, turn)
        PROPERTY(float, power)

        PROPERTY(WALK_ControlStatus_Mode, controlStatus)
        PROPERTY(Odometry, odometry)
        PROPERTY(bool, odometry_mask)
        PROPERTY(bool, pad1)
        PROPERTY(int16_t, pad2)

#ifdef __cplusplus
    wb_walk_controlstatus(WALK_ControlStatus_Mode c = WALK_Disconnected, float forward = 0, float left = 0, float turn = 0, float power = 0): _forward(forward), _left(left), _turn(turn), _power(power), _controlStatus(c), _odometry_mask(false) {}
#endif

};

#endif //_wb_walk_controlstatus_h
