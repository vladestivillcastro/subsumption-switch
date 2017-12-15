/* 
 * File:   VisionLines.h
 * Author: eugene
 *
 * Created on 1 December 2013, 8:02 PM
 */

#ifndef VisionLines_DEFINED
#define	VisionLines_DEFINED

#include <vector>
#include <string>
#include <SimpleShapes.h>

#include "wb_lines.h"
#include "Vision_Control.h"

namespace guWhiteboard {
class VisionLines : public wb_lines {
public:
    VisionLines() : wb_lines() {}
	
	
	VisionLines(std::string s) : wb_lines() {
		//NYI
	}
	
	bool addLine(const wb_line &line, VisionCamera camera) {
		for(int i = 0; i<6; ++i) {
			if(camera == Top) {
				if(!(topMask() & (1 << i))) {
					set_topLines(line, i);
					set_topMask(topMask() | (1 << i));
					return true;
				}	
			}
			else {
				if(!(bottomMask() & (1 << i))) {
					set_bottomLines(line, i);
					set_bottomMask(bottomMask() | (1 << i));
					return true;
				}
			}
		}
		return false;
	}
	
	std::vector<wb_line> topLines() const {
		std::vector<wb_line> result;
		for(int i = 0; i<6; ++i) {
			if(topMask() & (1 << i)) {
				result.push_back(wb_lines::topLines(i));
			}
		}
		return result;
	}
	
	std::vector<wb_line> bottomLines() const {
		std::vector<wb_line> result;
		for(int i = 0; i<6; ++i) {
			if(bottomMask() & (1 << i)) {
				result.push_back(wb_lines::bottomLines(i));
			}
		}
		return result;
	}
    
	void reset() {
		set_topMask(0);
		set_bottomMask(0);
	}
	
	std::string description() const {
		std::stringstream result;
		result << topLines().size() << "," << bottomLines().size();
		return result.str();
	}
	
};
}

#endif	/* VisionBall_DEFINED */
