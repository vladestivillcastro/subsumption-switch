#ifndef _WBTELEOPERATION_
#define _WBTELEOPERATION_

#include <sys/types.h>
#include <gu_util.h>
#include "MOTION_Interface.h"
#include "Vision_Control.h"

struct wb_teleoperation
{
	unsigned initBit : 1;
	PROPERTY(int, fowardWalkSpeed)
	PROPERTY(float, turnWalkSpeed)
	PROPERTY(float, headPitch)
	PROPERTY(float, headYaw)

#ifdef __cplusplus
	wb_teleoperation() : initBit(0), _fowardWalkSpeed(0), _turnWalkSpeed(0), _headPitch(0), _headYaw(0){}
#endif
};

#endif
