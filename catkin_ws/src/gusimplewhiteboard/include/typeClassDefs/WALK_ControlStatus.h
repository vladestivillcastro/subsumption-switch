/**                                                                     
 *  /file WALK_ControlStatus.h 
 *                                                                      
 *  Created by Carl Lusty in 2014.                                      
 *  Copyright (c) 2014 Carl Lusty                                       
 *  All rights reserved.                                                
 */                                                                     
                                                                         
#ifndef WALK_ControlStatus_DEFINED
#define WALK_ControlStatus_DEFINED


#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
#include <string>
#include <sstream>
#include <ctype.h>
#endif

#include <float.h>
#include "wb_walk_controlstatus.h"

namespace guWhiteboard
{
#define WALK_Disconnect WALK_Disconnected ///< alias for WALK_Disconnected

#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
        static const char *WALK_statusNames[] =
        {
                "disconnected", "stopped", "ready", "walking", "odometry"
        };
        static const char *WALK_commandNames[] =
        {
                "disconnect", "stop", "connect", "walk", "set_odometry"
        };
#endif // WHITEBOARD_POSTER_STRING_CONVERSION
        /**
         * Nao Walk control and status class
         */
        class WALK_ControlStatus : public wb_walk_controlstatus
        {

        public:
                /** designated constructor */
                WALK_ControlStatus(WALK_ControlStatus_Mode c = WALK_Disconnected, float forward = 0, float left = 0, float turn = 0, float power = 0): wb_walk_controlstatus(c, forward, left, turn, power) {}

                /** copy constructor */
                WALK_ControlStatus(const WALK_ControlStatus &other) : wb_walk_controlstatus(other.controlStatus(), other.forward(), other.left(), other.turn(), other.power()) { }

                /** copy assignment operator */
                WALK_ControlStatus &operator=(const WALK_ControlStatus &other) { set_controlStatus(other.controlStatus()); set_forward(other.forward()); set_left(other.left()); set_turn(other.turn()); set_power(other.power()); return *this; }

		inline bool operator == (const WALK_ControlStatus &s) 
		{
    			return (fabs(forward() - s.forward()) < FLT_EPSILON &&
				fabs(left() - s.left()) < FLT_EPSILON &&
				fabs(turn() - s.turn()) < FLT_EPSILON &&
				fabs(power() - s.power()) < FLT_EPSILON &&
				controlStatus()	== s.controlStatus() &&
				odometry() 	== s.odometry());
		}

		inline bool operator != (const WALK_ControlStatus &s) 
		{
			return !((*this) == s);
		}
		
            
#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
                /** string constructor */
                WALK_ControlStatus(const std::string &command) { from_string(command); }

                /** convert to a string */
                std::string description() const
                {
                        std::ostringstream ss;
                        int statval = controlStatus();
                        if (statval < 0 || statval > WALK_Ignore)
                                ss << "Corrupt WALK control status value " << statval;
                        else
                                ss << WALK_statusNames[statval];

                        if (controlStatus() == WALK_Run)
                                ss << "(" << forward() << "," << left() << "," << turn() << "," << power() << ")";

                        if (odometry_mask())
                                ss << " @(" << odometry().forward << "," << odometry().left << "," << odometry().turn << ")";

                        return ss.str();
                }

                /** convert from a string */
                void from_string(const std::string &command)
                {
                        const char *ccmd = command.c_str();
                        set_odometry_mask(false);
                        for (int cmd = WALK_Disconnected; cmd <= WALK_Ignore; cmd++)
                        {
                                if (strstr(ccmd, WALK_statusNames[cmd]) ||
                                    strstr(ccmd, WALK_commandNames[cmd]))
                                {
                                        set_controlStatus(WALK_ControlStatus_Mode(cmd));
                                        if (cmd != WALK_Run && cmd != WALK_Ignore)
                                                return;
                                        std::istringstream iss(command);
                                        std::string token;
                                        if (getline(iss, token, ','))
                                        {
                                                const char *str = token.c_str();
                                                while (*str != '-' && !isdigit(*str))
                                                {
                                                        if (!*str) break;
                                                        str++;
                                                }
                                                set_forward(float(atof(str)));
                                        }
                                        else set_forward(0);

                                        if (getline(iss, token, ','))
                                        {
                                                const char *str = token.c_str();
                                                while (*str != '-' && !isdigit(*str))
                                                {
                                                        if (!*str) break;
                                                        str++;
                                                }
                                                set_left(float(atof(str)));
                                        }
                                        else set_left(0);

                                        if (getline(iss, token, ','))
                                        {
                                                const char *str = token.c_str();
                                                while (*str != '-' && !isdigit(*str))
                                                {
                                                        if (!*str) break;
                                                        str++;
                                                }
                                                set_turn(float(atof(str)));
                                        }
                                        else set_turn(0);

                                        if (getline(iss, token, '@'))
                                        {
                                                const char *str = token.c_str();
                                                while (*str != '-' && !isdigit(*str))
                                                {
                                                        if (!*str) break;
                                                        str++;
                                                }
                                                set_power(float(atof(str)));
                                        }
                                        else set_power(0);

                                        /*
                                         * odometry after the '@'
                                         */
                                        if (getline(iss, token, ','))
                                        {
                                                const char *str = token.c_str();
                                                while (*str != '-' && !isdigit(*str))
                                                {
                                                        if (!*str) break;
                                                        str++;
                                                }
                                                set_odometry_mask(true);
                                                _odometry.forward = float(atof(str));
                                        }
                                        else _odometry.forward = 0.0f;

                                        if (getline(iss, token, ','))
                                        {
                                                const char *str = token.c_str();
                                                while (*str != '-' && !isdigit(*str))
                                                {
                                                        if (!*str) break;
                                                        str++;
                                                }
                                                _odometry.left = float(atof(str));
                                        }
                                        else _odometry.left = 0.0f;

                                        if (getline(iss, token, ')'))
                                        {
                                                const char *str = token.c_str();
                                                while (*str != '-' && !isdigit(*str))
                                                {
                                                        if (!*str) break;
                                                        str++;
                                                }
                                                _odometry.turn = float(atof(str));
                                        }
                                        else _odometry.turn = 0.0f;
                                }
                        }
                }
#endif // WHITEBOARD_POSTER_STRING_CONVERSION
        };
}

#endif // WALK_ControlStatus_DEFINED
