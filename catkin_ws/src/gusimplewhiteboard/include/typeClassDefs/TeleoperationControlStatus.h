/* 
 * File:   TeleoperationControlStatus.h
 * Author: eugene
 *
 * Created on 10 February 2014, 1:44 PM
 */

#ifndef TeleoperationControlStatus_DEFINED
#define	TeleoperationControlStatus_DEFINED

#include "MOTION_Interface.h"
#include "Vision_Control.h"
#include "wb_teleoperation.h"
#include <gu_util.h>
namespace guWhiteboard
{
class TeleoperationControlStatus : public wb_teleoperation {
public:
	TeleoperationControlStatus() {
		VERSION_PROPERTY_INITIALIZATION(action)
		VERSION_PROPERTY_INITIALIZATION(stance)
	}
	bool connected() {
		return initBit == 1;
	}
	
	void connect() {
		initBit = 1;
	}
	
	void disconnect() {
		initBit = 0;
	}
	
	void resetVersions() {
		reset_action_version();
		reset_stance_version();
	}
	
private:
	VERSIONED_PROPERTY(guWhiteboard::Motions::action, action)
	VERSIONED_PROPERTY(guWhiteboard::Motions::stance, stance)
	PROPERTY(StreamingType, streamType)
	PROPERTY(VisionCamera, selectedCamera)
};
}
#endif	/* TeleoperationControlStatus_DEFINED */

