/* 
 * File:   VisionBall.h
 * Author: eugene
 *
 * Created on 25 November 2013, 6:03 PM
 */

#ifndef VisionBall_DEFINED
#define	VisionBall_DEFINED

#include <string>
#include <cstdlib>

#include <SimpleShapes.h>
#include "Vision_Control.h"
#include "wb_ball.h"



namespace guWhiteboard {
class VisionBall {
	PROPERTY(uint64_t, frameNumber)
	PROPERTY(wb_ball, topBall)
	PROPERTY(wb_ball, bottomBall)
        PROPERTY(bool, topVisible)
        PROPERTY(bool, bottomVisible)
        PROPERTY(int16_t, pad)                  ///< align to 32 bits
public:
    VisionBall() : _frameNumber(0), _topBall(), _bottomBall() {
    }
	
	VisionBall &operator=(const VisionBall& a) {
		_frameNumber = a.frameNumber();
		_topBall = a.topBall();
		_bottomBall = a.bottomBall();
		set_topVisible(a.topVisible());
		set_bottomVisible(a.bottomVisible());
		return *this;
	}
	
	VisionBall(const VisionBall &ball) {
		_frameNumber = ball.frameNumber();
		_topBall = ball.topBall();
		_bottomBall = ball.bottomBall();
		set_topVisible(ball.topVisible());
		set_bottomVisible(ball.bottomVisible());
	}
	
	/*DEPRICATED*/
	std::bitset<2> getMask() const {
		std::bitset<2> objectMask;
		objectMask[Top] = _topVisible;
		objectMask[Bottom] = _bottomVisible;
		return objectMask;
	}

	void setBall(SimpleCircle ballInfo, VisionCamera camera) {
		if(camera == Top) {
			topBall().set_x(ballInfo.GetCenter().x);
			topBall().set_y(ballInfo.GetCenter().y);
			topBall().set_radius(ballInfo.GetRadius());
			set_topVisible(true);
		}
		if(camera == Bottom) {
			bottomBall().set_x(ballInfo.GetCenter().x);
			bottomBall().set_y(ballInfo.GetCenter().y);
			bottomBall().set_radius(ballInfo.GetRadius());
                    
			set_bottomVisible(true);
		}
	}
	
	void setBall(wb_ball ballInfo, VisionCamera camera) {
		if(camera == Top) {
			topBall().set_x(ballInfo.x());
			topBall().set_y(ballInfo.y());
                        topBall().set_radius(ballInfo.radius());
                        set_topVisible(true);
		}
		if(camera == Bottom) {
			bottomBall().set_x(ballInfo.x());
			bottomBall().set_y(ballInfo.y());
			bottomBall().set_radius(ballInfo.radius());
                        set_bottomVisible(true);

                }
	}

	
	int16_t topRadius() const { return topBall().radius(); }
	int16_t topX() const { return topBall().x(); }
	int16_t topY() const { return topBall().y(); }
	int16_t bottomRadius() const { return bottomBall().radius(); }
	int16_t bottomX() const { return bottomBall().x(); }
	int16_t bottomY() const { return bottomBall().y(); }
	bool visible() {return topVisible() || bottomVisible(); }
	int16_t radius() const { 
		if (topVisible())
			return topBall().radius();
		else if (bottomVisible())
			return bottomBall().radius();
		else
			return 0;
	}
	int16_t x() const { 
                if (topVisible())
			return topBall().x();
                else if (bottomVisible())
			return bottomBall().x();
		else
			return 0;
	}
	int16_t y() const { 
                if (topVisible())
                    return topBall().y();
                else if (bottomVisible())
                    return bottomBall().y();
		else
			return 0;
	}
	
	
	void Reset() {
		set_topVisible(false);
		set_bottomVisible(false);
	}

#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
        
        static const char SEPARATOR_IS_AT = '@';

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat"
        VisionBall(std::string s): VisionBall() {
                from_string(s);
        }

        VisionBall(const char *s): VisionBall() {
                from_string(std::string(s));
        }
#pragma clang diagnostic pop
        
	void from_string(std::string s) {
		std::string radiousDel (1,SEPARATOR_IS_AT);
		set_frameNumber(0);
		Reset();
		size_t n = -4;
		std::string command = "BALL";
		std::transform(s.begin(), s.end(), s.begin(), ::toupper);
		while(n!=std::string::npos) {
			n = s.find(command, n+4);
			if (n!=std::string::npos && n>=3) {
				std::string t = s.substr(n+command.length()+1);
				SimpleCircle ballInfo;
				VisionCamera cam;
				if(s.substr(n-3, 3).find("TOP") != std::string::npos)
					cam = Top;
				else
					cam = Bottom;
				
				ballInfo.setCenter(t.c_str());
                                 std::size_t found = t.find(SEPARATOR_IS_AT);
                                if (std::string::npos!=found )
                                { std::string strRadious=t.substr (found+1);
                                        ballInfo.setRadius(static_cast<u_int16_t>(::atoi(strRadious.c_str())));
                                }
				setBall(ballInfo, cam);
			}
		}
		std::string fnum = "FRAMENUMBER";
		n = s.find(fnum);
		if(n != std::string::npos) {
			
			set_frameNumber(atol(s.substr(n+fnum.length()+1).c_str()));
		}
	}
	
	std::string description() {
		std::stringstream result;
		if(_topVisible) {
			result << "TopBall:(" << topBall().x() << "," << topBall().y() << ")"<< SEPARATOR_IS_AT << topBall().radius();
		}
		if(_bottomVisible) {
			result << "BottomBall:(" << bottomBall().x() << "," << bottomBall().y() << ")"<< SEPARATOR_IS_AT << bottomBall().radius();
		}
		result << "FrameNumber=" << frameNumber();
		return result.str();
	}
#endif // WHITEBOARD_POSTER_STRING_CONVERSION

};
}

#endif	/* VisionBall_DEFINED */

