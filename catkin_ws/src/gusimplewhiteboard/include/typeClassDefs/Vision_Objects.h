/*
 * VisionObjects.h
 *
 *  Created on: 23/04/2013
 *      Author: eugene
 */

#include <bitset>
#include <gu_util.h>
#include <SimpleShapes.h>
#include <stdint.h>
#include <sstream>
#include <stdio.h>
#include <string>
#include <algorithm>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"

#ifndef VisionObjects_DEFINED
#define VisionObjects_DEFINED

struct WbBallInfo {
	uint16_t radius;
	GUPoint<int16_t> position;
};

struct WbGoalPostInfo {
	GUPoint<int16_t> outerBottom;
	GUPoint<int16_t> outerTop;

	GUPoint<int16_t> innerBottom;
	GUPoint<int16_t> innerTop;
};

struct WbLineInfo {
	uint16_t startX, startY, endX, endY;
};


namespace VisionObjectTypes {enum object {
	Ball = 0,
	LeftGoalPost,
	RightGoalPost,
	Line1,
	Line2,
	Line3,
	Line4,
	Line5,
	NUM_VISION_OBJECTS
};}

inline GUPoint<int16_t> getPoint(std::string *str) {
	GUPoint<int16_t> point;
	point.x = static_cast<int16_t>(atoi(str->substr(1, str->find(",")-1).c_str()));
	size_t pointEnd = size_t(str->find(")")-1);
	point.y = static_cast<int16_t>(atoi(str->substr(str->find(",")+1, pointEnd).c_str()));
	pointEnd += 2;
	*str = str->substr(pointEnd, str->length()-pointEnd);
	return point;
}

#define NUM_LINES 5

static const char* Objects[] = {"BALL", "LEFTGOAL", "RIGHTGOAL", "LINE1", "LINE2", "LINE3", "LINE4", "LINE5"};
namespace guWhiteboard
{
class VisionObjects {
public:
	std::bitset<VisionObjectTypes::NUM_VISION_OBJECTS> objectMask;
	SimpleLine _lines[NUM_LINES];
	WbBallInfo _ball;
	WbGoalPostInfo _leftGoalPost;
	WbGoalPostInfo _rightGoalPost;
        unsigned long frameNumber;
public:
	VisionObjects() : frameNumber(0){
		objectMask.reset();
	}

        VisionObjects(const VisionObjects &other) { *this = other; }
	VisionObjects &operator=(const VisionObjects& a) {
		objectMask = a.objectMask;
		for(int i = 0; i<NUM_LINES; ++i) {
				_lines[i].start = a._lines[i].start;
				_lines[i].end = a._lines[i].end;
		}
		_ball = a._ball;
		_leftGoalPost = a._leftGoalPost;
		_rightGoalPost = a._rightGoalPost;
		frameNumber = a.frameNumber;
		return *this;
	}

public:
        VisionObjects(std::string s) : frameNumber(0) {
		objectMask.reset();
		size_t n;
		std::string command;
		std::transform(s.begin(), s.end(), s.begin(), ::toupper);
		for (unsigned i = 0; i<VisionObjectTypes::NUM_VISION_OBJECTS; ++i) {
			command = std::string(Objects[i]);
			n = s.find(command);
			if (n!=std::string::npos) {
                                std::string t = s.substr(n+command.length()+1);
                                WbGoalPostInfo goalInfo;
				WbBallInfo ballInfo;
                                switch(i) {
                                    case VisionObjectTypes::LeftGoalPost://formate of LeftGoal=(9,9)(9,9)(9,9)(9,9)
                                        goalInfo.outerBottom = getPoint(&t);
                                        goalInfo.outerTop = getPoint(&t);
                                        goalInfo.innerBottom = getPoint(&t);
                                        goalInfo.innerTop = getPoint(&t);
                                        setLeftGoalPost(goalInfo);
                                        break;
                                    case VisionObjectTypes::RightGoalPost:
                                        goalInfo.outerBottom = getPoint(&t);
                                        goalInfo.outerTop = getPoint(&t);
                                        goalInfo.innerBottom = getPoint(&t);
                                        goalInfo.innerTop = getPoint(&t);
                                        setRightGoalPost(goalInfo);
                                        break;
				    case VisionObjectTypes::Ball: //formate of Ball=(x,y)@r
					ballInfo.position = getPoint(&t);
					ballInfo.radius = u_int16_t(atoi(t.substr(1, t.substr(1, t.size()-2).find_first_not_of("-0123456789")).c_str()));
					setBall(ballInfo);
					break;
                                }
                        }
                }
        }
        
	void setBall(WbBallInfo ballInfo) {
		_ball = ballInfo;
		objectMask[VisionObjectTypes::Ball]  = 1;
	}
	const WbBallInfo *ball() const
        {
		if(objectMask[VisionObjectTypes::Ball])
			return &_ball;
		return NULL;
	}
	WbBallInfo *ball() {
		if(objectMask[VisionObjectTypes::Ball])
			return &_ball;
		return NULL;
	}
	void setLeftGoalPost(WbGoalPostInfo postInfo) {
		_leftGoalPost = postInfo;
		objectMask[VisionObjectTypes::LeftGoalPost]  = 1;
	}
	const WbGoalPostInfo* LeftGoalPost() const
        {
		if(objectMask[VisionObjectTypes::LeftGoalPost])
			return &_leftGoalPost;
		return NULL;
	}
	WbGoalPostInfo *LeftGoalPost()
        {
		if(objectMask[VisionObjectTypes::LeftGoalPost])
			return &_leftGoalPost;
		return NULL;
	}

	void setRightGoalPost(WbGoalPostInfo postInfo) {
		_rightGoalPost = postInfo;
		objectMask[VisionObjectTypes::RightGoalPost]  = 1;
	}
	const WbGoalPostInfo *RightGoalPost() const
        {
		if(objectMask[VisionObjectTypes::RightGoalPost])
			return &_rightGoalPost;
		return NULL;
	}
	WbGoalPostInfo *RightGoalPost()
        {
		if(objectMask[VisionObjectTypes::RightGoalPost])
			return &_rightGoalPost;
		return NULL;
	}

	void addLine(SimpleLine line) {
		for(size_t i = VisionObjectTypes::Line1; i<VisionObjectTypes::Line5; ++i) {
			if(objectMask[i] == 0) {
				_lines[i-VisionObjectTypes::Line1] = line;
				objectMask[i] = 1;
				break;
			}
		}
	}

	const SimpleLine *Line(VisionObjectTypes::object line) const
        {
		if(line >= VisionObjectTypes::Line1 && line <= VisionObjectTypes::Line5)
			if(objectMask[line])
				return & _lines[line - VisionObjectTypes::Line1];
		return NULL;
	}
	SimpleLine *Line(VisionObjectTypes::object line)
        {
		if(line >= VisionObjectTypes::Line1 && line <= VisionObjectTypes::Line5)
			if(objectMask[line])
				return & _lines[line - VisionObjectTypes::Line1];
		return NULL;
	}
        void setFrameNumber(unsigned long fn) {
            frameNumber = fn;
        }
        
        unsigned long FrameNumber() const
        {
            return frameNumber;
        }


	void ClearMask() {
		objectMask.reset();
	}
	
	std::bitset<VisionObjectTypes::NUM_VISION_OBJECTS> Mask() {
		return this->objectMask;
	}
#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
	std::string description() {
		std::stringstream result;
		if(objectMask[VisionObjectTypes::Ball]) {
			result << "Ball:(" << ball()->position.x << "," << ball()->position.y << "," << ball()->radius << ") ";
		}
		if(objectMask[VisionObjectTypes::LeftGoalPost])
			result << "LeftGoalPost:(" << _leftGoalPost.outerBottom.x << "," << _leftGoalPost.outerBottom.y << ","
			<< _leftGoalPost.outerTop.x << "," << _leftGoalPost.outerTop.y << ","
			<< _leftGoalPost.innerBottom.x << "," << _leftGoalPost.innerBottom.y << ","
			<< _leftGoalPost.innerTop.x << "," << _leftGoalPost.innerTop.y << ") ";
		if(objectMask[VisionObjectTypes::RightGoalPost])
			result << "RightGoalPost:(" << _rightGoalPost.outerBottom.x << "," << _rightGoalPost.outerBottom.y << ","
			<< _rightGoalPost.outerTop.x << "," << _rightGoalPost.outerTop.y << ","
			<< _rightGoalPost.innerBottom.x << "," << _rightGoalPost.innerBottom.y << ","
			<< _rightGoalPost.innerTop.x << "," << _rightGoalPost.innerTop.y << ") ";
		for(int i = VisionObjectTypes::Line1; i<=VisionObjectTypes::Line5; ++i)
			if(objectMask[i])
				result << "Line" << (i-VisionObjectTypes::Line1+1) << "(" << _lines[i-VisionObjectTypes::Line1].start.x
				<< "," << _lines[i-VisionObjectTypes::Line1].start.y
				<< "," << _lines[i-VisionObjectTypes::Line1].end.x
				<< "," << _lines[i-VisionObjectTypes::Line1].end.y << ") ";
                result << "FrameNumber=" << frameNumber;
		return result.str();
	}
#endif
};
}



#endif /* VisionObjects_DEFINED */
