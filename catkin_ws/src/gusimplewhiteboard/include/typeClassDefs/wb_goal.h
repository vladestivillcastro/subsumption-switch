#ifndef _WBGOAL_
#define _WBGOAL_

#include <sys/types.h>
#include <gu_util.h>
#include <SimpleShapes.h>

struct wb_goal
{
	PROPERTY(GUPoint<int16_t>, outerBottom)
	PROPERTY(GUPoint<int16_t>, outerTop)
	PROPERTY(GUPoint<int16_t>, innerBottom)
	PROPERTY(GUPoint<int16_t>, innerTop)
        PROPERTY(bool, visible)
        PROPERTY(bool, pad)                         ///< align to 16 bits
#ifdef __cplusplus
	wb_goal() : _outerBottom(), _outerTop(), _innerBottom(), _innerTop(), _visible(false), _pad(false) {}
#endif
};

#endif
