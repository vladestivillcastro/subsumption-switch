/* 
 * File:   WalkDaata.h
 * Author: eugene
 *
 * Created on 5 December 2013, 11:19 AM
 */
#include <string>

#ifndef WalkData_DEFINED
#define	WalkData_DEFINED

namespace guWhiteboard {

class WalkData {
public:
	float data[4][7];
	int numPoints;
	WalkData() {}
	WalkData(std::string) {
		for(int i = 0; i<3; ++i) {
			for(int j = 0; j<4; ++j) {
				data[j][i] = i*j;
			}
		}
	}
	
	std::string description() {
		return std::string("NYI");
	}
};
}

#endif	/* WalkData_DEFINED */

