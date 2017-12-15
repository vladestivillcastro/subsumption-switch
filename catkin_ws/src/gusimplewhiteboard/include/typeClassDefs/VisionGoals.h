/* 
 * File:   VisionGoals.h
 * Author: eugene
 *
 * Created on 1 December 2013, 8:02 PM
 */

#ifndef VisionGoals_DEFINED
#define	VisionGoals_DEFINED

#include <SimpleShapes.h>
#include <string>
#include <gu_util.h>
#include "wb_goal.h"

#include "Vision_Control.h"

namespace guWhiteboard {
class VisionGoals {
private:
	wb_goal topLeft;//top camera
	wb_goal topRight;
	wb_goal bottomLeft;//bottom camera
	wb_goal bottomRight;
	unsigned long _frameNumber;
public:
    VisionGoals() : topLeft(), topRight(), bottomLeft(), bottomRight(), _frameNumber(0) {
    }
	
	VisionGoals(std::string s) : topLeft(), topRight(), bottomLeft(), bottomRight(), _frameNumber(0) {
		
		size_t n = static_cast<size_t>(-8);
		std::string command = "LEFTPOST";
		std::transform(s.begin(), s.end(), s.begin(), ::toupper);
		while(n!=std::string::npos) {
			n = s.find(command, n+8);
			if (n!=std::string::npos) {
				std::string t = s.substr(n+command.length()+1);
				wb_goal postInfo;
				VisionCamera cam;
				if(s.substr(n-3, 3).find("TOP") != std::string::npos)
					cam = Top;
				else
					cam = Bottom;
				
				postInfo.set_outerBottom(t.c_str());
				postInfo.set_outerTop(t.c_str());
				postInfo.set_innerBottom(t.c_str());
				postInfo.set_innerTop(t.c_str());
				setLeftGoalPost(postInfo, cam);
			}
		}
		
		n = static_cast<size_t>(-8);
		command = "RIGHTPOST";
		std::transform(s.begin(), s.end(), s.begin(), ::toupper);
		while(n!=std::string::npos) {
			n = s.find(command, n+8);
			if (n!=std::string::npos) {
				std::string t = s.substr(n+command.length()+1);
				wb_goal postInfo;
				VisionCamera cam;
				if(s.substr(n-3, 3).find("TOP") != std::string::npos)
					cam = Top;
				else
					cam = Bottom;
				
				std::vector<std::string> com = components_of_string_separated(t, '(');
				postInfo.set_outerBottom(com.at(1).c_str());
				postInfo.set_outerTop(com.at(2).c_str());
				postInfo.set_innerBottom(com.at(3).c_str());
				postInfo.set_innerTop(com.at(4).c_str());
				setRightGoalPost(postInfo, cam);
			}
		}
	}
	void setLeftGoalPost(wb_goal postInfo, VisionCamera camera) {
		if(camera == Top) {
			topLeft = postInfo;
			topLeft.set_visible(true);
		}
		else {
			bottomLeft = postInfo;
			bottomLeft.set_visible(true);
		}
	}
	
	void setRightGoalPost(wb_goal postInfo, VisionCamera camera) {
		if(camera == Top) {
			topRight = postInfo;
			topRight.set_visible(true);
		}
		else {
			bottomRight = postInfo;
			bottomRight.set_visible(true);
		}
	}
	
	const wb_goal &leftPost(VisionCamera camera) const
	{
		if(camera == Top)
			return topLeft;
		else
			return bottomLeft;
	}
	
	const wb_goal &rightPost(VisionCamera camera) const
	{
		if(camera == Top)
			return topRight;
		else
			return bottomRight;
	}
	
	wb_goal &leftPost(VisionCamera camera)
	{
		if(camera == Top)
			return topLeft;
		else
			return bottomLeft;
	}
	
	wb_goal &rightPost(VisionCamera camera)
	{
		if(camera == Top)
			return topRight;
		else
			return bottomRight;
	}
	
	void Reset() {
		topLeft.set_visible(false);
		topRight.set_visible(false);
		bottomLeft.set_visible(false);
		bottomRight.set_visible(false);
	}
	
	void setFrameNumber(unsigned long fn) {
		_frameNumber = fn;
	}
	
	unsigned long frameNumber() const {
		return _frameNumber;
	}
	
	std::string description() {
		std::stringstream result;
		
		if(bottomLeft.visible())
			result << "BottomLeftPost:(" << bottomLeft.outerBottom().x << "," << bottomLeft.outerBottom().y << ")("
			<< bottomLeft.outerTop().x << "," << bottomLeft.outerTop().y << ")("
			<< bottomLeft.innerBottom().x << "," << bottomLeft.innerBottom().y << ")("
			<< bottomLeft.innerTop().x << "," << bottomLeft.innerTop().y << ") ";
		if(topLeft.visible())
			result << "TopLeftPost:(" << topLeft.outerBottom().x << "," << topLeft.outerBottom().y << ")("
			<< topLeft.outerTop().x << "," << topLeft.outerTop().y << ")("
			<< topLeft.innerBottom().x << "," << topLeft.innerBottom().y << ")("
			<< topLeft.innerTop().x << "," << topLeft.innerTop().y << ") ";
		if(bottomRight.visible())
			result << "BottomRightPost:(" << bottomRight.outerBottom().x << "," << bottomRight.outerBottom().y << ")("
			<< bottomRight.outerTop().x << "," << bottomRight.outerTop().y << ")("
			<< bottomRight.innerBottom().x << "," << bottomRight.innerBottom().y << ")("
			<< bottomRight.innerTop().x << "," << bottomRight.innerTop().y << ") ";
		if(topRight.visible())
			result << "TopRightPost:(" << topRight.outerBottom().x << "," << topRight.outerBottom().y << ")("
			<< topRight.outerTop().x << "," << topRight.outerTop().y << ")("
			<< topRight.innerBottom().x << "," << topRight.innerBottom().y << ")("
			<< topRight.innerTop().x << "," << topRight.innerTop().y << ") ";
		return result.str();
	}
	
};
}

#endif	/* VisionGoals_DEFINED */

