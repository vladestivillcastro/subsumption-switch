/* 
 * File:   wb_lines.h
 * Author: eugene
 *
 * Created on 22 July 2014, 2:12 PM
 */

#ifndef WB_LINES_H
#define	WB_LINES_H

#include "wb_point.h"
	
struct wb_line {
	PROPERTY(wb_point2d, start)
	PROPERTY(wb_point2d, end)
#ifdef __cplusplus
	wb_line() : _start(), _end() {}
#endif
};
	
struct wb_lines {
	PROPERTY(uint64_t, frameNumber)
	ARRAY_PROPERTY(wb_line, topLines, 6)
	ARRAY_PROPERTY(wb_line, bottomLines, 6)
	PROPERTY(int8_t, topMask)
	PROPERTY(int8_t, bottomMask)
#ifdef __cplusplus
	wb_lines() : _frameNumber(0), _topMask(0), _bottomMask(0) {}
#endif
};


#endif	/* WB_LINES_H */

