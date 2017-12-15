#ifndef _WBBALL_
#define _WBBALL_

#include <sys/types.h>
#include <gu_util.h>

struct wb_ball
{
	PROPERTY(int16_t, x)
        PROPERTY(int16_t, y)
        PROPERTY(int16_t, radius)
#ifdef __cplusplus
	wb_ball() : _x(0), _y(0), _radius(0) {}
#endif
};

#endif
