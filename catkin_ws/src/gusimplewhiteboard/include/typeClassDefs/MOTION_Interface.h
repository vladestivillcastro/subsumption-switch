/**                                                                     
 *  /file MOTION_Interface.h
 *                                                                      
 *  Created by Carl Lusty in 2013.                                      
 *  Copyright (c) 2013 Carl Lusty                                       
 *  All rights reserved.                                                
 */                                                                     

                                                                        
#ifndef MOTION_Interface_DEFINED
#define MOTION_Interface_DEFINED
#define MOTION_Commands_DEFINED
#define MOTION_Status_DEFINED

#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
#include <cctype>
#include <cstdlib>
#endif // WHITEBOARD_POSTER_STRING_CONVERSION

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <gu_util.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"

#define MOTION_DIR "/data/motions/"
#define MOTION_FILE_POSTFIX ".mot"
#define JOINT_CHAIN_MAXSIZE 10
#define CHAIN_IGNORED_MOTION_DURATION 100 //cycles
#define TRANSITION_NAME(s, e) TRANSITION_NAME_FROM_STRING(stance_strings[s], stance_strings[e])
#define TRANSITION_NAME_FROM_STRING(s, e) ((((std::string("transition_") += s) += "_") += e) += MOTION_FILE_POSTFIX)
#define ACTION_NAME(a, s) ((((std::string("action_") += stance_strings[s]) += "_") += action_strings[a]) += MOTION_FILE_POSTFIX)
#define STANCE_NAME(s) STANCE_NAME_FROM_STRING(stance_strings[s])
#define STANCE_NAME_FROM_STRING(s) std::string("stance_").append(std::string(s).append(MOTION_FILE_POSTFIX))

#define MAP_ACTION_ENTRY(a, s, c) v[a] = Action_Transition(a, s, c);

#define MY_CHECK_ENUM(s,p,v)       if (strstr((s), #v)) p = v


namespace guWhiteboard                                                  
{
        namespace Motions
        {
                enum motion_file_layout
                {
                        NAO_Version = 0,        //V3 = 0, V4 = 1
                        Units,                  //RAD = 0 or DEG = 1
                        Stiffness,              //0 - 100
                        Joints,                 //Joint numbers, see SensorInfo.h 'jointNames' array
                        Motions                 //The rest of the file is comma seperated joints and 10ms seperated lines
                };

                enum stance
                {
                        Standby_stance = 0, // releases stiffness when done
                        Kneeling_stance, // default stance - no stiffness by default
                        Standing_stance,
                        GoalieSaveLeft_stance,
                        GoalieSaveRight_stance,
                        FallenForward_stance, // no path to the fallen stances, these can only be gotten to by the fall manager FSM overwriting the current stance
                        FallenBack_stance,
                        NUM_OF_STANCES
                };

                enum action
                {
                        Kneeling_wave = 0,
            		Kneeling_quickwave,
                        Kneeling_flagwave, // Jeremy's Flag Wave
                        Standing_leftkick,
                        Standing_rightkick,
                        Standing_leftpass,
                        Standing_rightpass,
                        Standing_wave,
                        NUM_OF_ACTIONS
                };
                
                static const char *stance_strings[NUM_OF_STANCES] =
                {
                        "Standby_stance",
                        "Kneeling_stance",
                        "Standing_stance",
                        "GoalieSaveLeft_stance",
                        "GoalieSaveRight_stance",
                        "FallenForward_stance",
                        "FallenBack_stance"
                };

                static const char *action_strings[NUM_OF_ACTIONS] =
                {
                        "Kneeling_wave",
                        "Kneeling_quickwave",
                        "Kneeling_flagwave", // Jeremy's Flag Wave
                        "Standing_leftkick",
                        "Standing_rightkick",
                        "Standing_leftpass",
                        "Standing_rightpass",
                        "Standing_wave"
                };

                struct Stance_Transition
                {
                        stance _s;
                        stance _e;
                        int _cost;
                        std::string _name;
                        Stance_Transition(stance s, stance e, int cost) : _s(s), _e(e), _cost(cost), _name(TRANSITION_NAME(s, e)) { }
                };

                struct T{
                        static std::vector<Stance_Transition> create_transitions() //many paths to a stance will work (picks shortest by using the cost)
                        {
                                std::vector<Stance_Transition> v;
                                v.push_back(Stance_Transition(Standby_stance,           Kneeling_stance,        230));
                                v.push_back(Stance_Transition(Kneeling_stance,          Standby_stance,         300));
                                v.push_back(Stance_Transition(Kneeling_stance,          Standing_stance,        150));
                                v.push_back(Stance_Transition(Standing_stance,          Kneeling_stance,        100)); 
                                v.push_back(Stance_Transition(Standing_stance,  	GoalieSaveLeft_stance,  1));
                                v.push_back(Stance_Transition(Standing_stance,  	GoalieSaveRight_stance, 1));
                                v.push_back(Stance_Transition(FallenForward_stance,     Standby_stance,         400));
                                v.push_back(Stance_Transition(FallenBack_stance, 	Kneeling_stance,        500));
                                v.push_back(Stance_Transition(Kneeling_stance,          GoalieSaveLeft_stance,  150));
                                v.push_back(Stance_Transition(GoalieSaveLeft_stance,    Kneeling_stance,        225));
                                v.push_back(Stance_Transition(Kneeling_stance,          GoalieSaveRight_stance, 480));
                                v.push_back(Stance_Transition(GoalieSaveRight_stance,   Kneeling_stance,        225));

                                return v;
                        }
                        static const std::vector<Stance_Transition> _transitions;
                };

                struct Action_Transition
                {
                        action _a; //not really needed here
                        stance _s;
                        int _cost;                        
                        std::string _name;
                        Action_Transition(action a = Kneeling_wave, stance s = Kneeling_stance, int cost = 1): _a(a), _s(s), _cost(cost), _name(ACTION_NAME(a, s)) { }
                };

                struct A{//CHANGE FROM MAP TO VECTOR AND USE THE ENUM INDEX TO GET THE ACTION_TRANSITION O(1)
                        static std::map<action, Action_Transition> create_actions() //each action may only have one stance
                        {
                                std::map<action, Action_Transition> v;
                                MAP_ACTION_ENTRY(Kneeling_wave,         Kneeling_stance, 1); //cost should be time to perform the action
                                MAP_ACTION_ENTRY(Kneeling_quickwave,    Kneeling_stance, 1); //cost should be time to perform the action
                                // Jeremy's Flag Wave Action. 
                                MAP_ACTION_ENTRY(Kneeling_flagwave,     Kneeling_stance, 1);
                                MAP_ACTION_ENTRY(Standing_leftkick,     Standing_stance, 443);
                                MAP_ACTION_ENTRY(Standing_rightkick,    Standing_stance, 443);
                                MAP_ACTION_ENTRY(Standing_leftpass,     Standing_stance, 1);
                                MAP_ACTION_ENTRY(Standing_rightpass,    Standing_stance, 1);
                                MAP_ACTION_ENTRY(Standing_wave,         Standing_stance, 1);

                                return v;
                        }
                        static const std::map<action, Action_Transition> _actions;
                };
                

                class Stance_Path
                {
                private:
                        int _cost;
                        int _off;
                        bool _reached;
                public:
                        u_int8_t _stances[JOINT_CHAIN_MAXSIZE];

                        Stance_Path(): _cost(0), _off(0), _reached(false) { }
                        int offset() { return _off; }
                        bool valid() { return _cost != -1 ? true : false; }
                        void invalidate() { _cost = -1; }
                        void reached() { _reached = true; }
                        int cost() { return _cost; }
                        void add_stance(stance s, int cost) { if(_off == (JOINT_CHAIN_MAXSIZE-1)) { invalidate(); } _stances[_off] = static_cast<u_int8_t>(s); _off++; _cost += cost; } //JOINT_CHAIN_MAXSIZE-1 leaves room for an action
                        void add_stance(stance s, int cost, int transition) { if(_off == (JOINT_CHAIN_MAXSIZE-1)) { invalidate(); } _stances[_off] = static_cast<u_int8_t>(transition+NUM_OF_STANCES+NUM_OF_ACTIONS); _off++; _stances[_off] = static_cast<u_int8_t>(s); _off++; _cost += cost; } //JOINT_CHAIN_MAXSIZE-1 leaves room for an action
                        void pretty_print()
                        {
                                std::stringstream ss;
                                ss << "Stance_Path: " << static_cast<int>(_off) << " ";
                                for(int i = 0; i < _off; i++)
                                {
                                        if(_stances[i] < NUM_OF_STANCES)
                                                ss << "-> " << static_cast<int>(_stances[i]) << " " << stance_strings[_stances[i]];
                                        else
                                                ss << " transition: " << static_cast<int>(_stances[i]) << " ";
                                }
                                fprintf(stderr, "%s\n", ss.str().c_str());
                        }
                        bool contains(stance s)
                        {
                                for(int i = 0; i < _off; i++)
                                {
                                        if(_stances[i] == s)
                                                return true;
                                }
                                return false;
                        }
                };
        }

	class MOTION_Status
	{
                BIT_PROPERTY(running)
                PROPERTY(int8_t, expected_stance)
                PROPERTY(int8_t, verified_stance) //NYI
		public:
		MOTION_Status(bool running, int8_t expected, int8_t verified)
		{
			_running = running;
			_expected_stance = expected;
			_verified_stance = verified;
		}

		bool isRunning() { return _running; }
		Motions::stance expectedStance() { return static_cast<Motions::stance>(_expected_stance); }
		Motions::stance verifiedStance() { return static_cast<Motions::stance>(_verified_stance); }

#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
                /// string constructor (see from_string() below)
                MOTION_Status(const std::string &str) { from_string(str); }

		void from_string(const std::string &str)
                {
			if(str.size() == 0)
				return;
			//NYI
                }
                std::string description() 
                {
                        std::stringstream ss;
			ss << "Running: " << isRunning() << ", ";

			ss << "Expected: ";
			Motions::stance e = expectedStance();
			if(e != Motions::NUM_OF_STANCES)
				ss << Motions::stance_strings[e];
			else
				ss << "unknown";	
			ss << ", ";

			ss << "Verified (NYI): ";
			Motions::stance v = verifiedStance();
			if(v != Motions::NUM_OF_STANCES)
				ss << Motions::stance_strings[v];
			else
				ss << "unknown";	

			return ss.str();
                }
#endif // WHITEBOARD_POSTER_STRING_CONVERSION
	};
        
        class MOTION_Commands
        {
                ARRAY_PROPERTY(u_int8_t, stance_action, JOINT_CHAIN_MAXSIZE) //private - of type stance_actions casted to an 8 bit int (saves 30 bytes of wb space)
                PROPERTY(int8_t, num_of_stance_actions)

                BIT_PROPERTY(stance_action_mask)
                BIT_PROPERTY(ignore_chain)
                BIT_PROPERTY(ignore_chain_mask) //go to the joint targets directly
                
                //Stance actions will set the stiffness to the value required in the motion file, these do not need to be called in general
                BIT_PROPERTY(head_stiffness)
                BIT_PROPERTY(body_stiffness)
                /* Control bits */
                BIT_PROPERTY(head_stiffness_mask)
                BIT_PROPERTY(body_stiffness_mask)

        public:                
                MOTION_Commands(bool head_stiffness = false, bool body_stiffness = false, bool masks = false/*, Motions::stance stance = Motions::Kneeling_stance*/)
                {
                        _head_stiffness = head_stiffness;
                        _body_stiffness = body_stiffness;
                        _stance_action_mask = false;
                        _ignore_chain_mask = false;
                        _head_stiffness_mask = masks;
                        _body_stiffness_mask = masks;
                        //goto stance stance_action! NYI
//                        _motion_player = motion_player;
                }

        private:
                Motions::Stance_Path decide_stance(Motions::stance current, Motions::stance desired, Motions::Stance_Path p)
                {
                        const std::vector<Motions::Stance_Transition> _transitions = Motions::T::create_transitions(); // move this out of here!

                        if(current == desired)
                        {
                                p.reached();
                                return p;                                
                        }

                        Motions::Stance_Path r;
                        r.invalidate();

                        for (unsigned i = 0; i < _transitions.size(); i++)
                        {
                                if(_transitions[i]._s == current && !p.contains(_transitions.at(i)._e)) // I can use this transiton from my current stance
                                {
                                        Motions::Stance_Path p2 = p;
                                        p2.add_stance(_transitions.at(i)._e, _transitions.at(i)._cost, int(i));

                                        Motions::Stance_Path c = decide_stance(_transitions.at(i)._e, desired, p2);
                                        if(!r.valid() || (c.cost() < r.cost() && c.valid()))
                                                r = c;
                                }
                        }

                        return r;
                }


        public:
                long GoToStance(Motions::stance current, Motions::stance desired)
                {
                        Motions::Stance_Path p;
                        p.add_stance(current, 1);                        
                        p = decide_stance(current, desired, p);
                        p.pretty_print();

                        memcpy(_stance_action, p._stances, sizeof(p._stances));
                        _num_of_stance_actions = static_cast<int8_t>(p.offset());
                        _stance_action_mask = true;

                        return p.cost();
                }

                long DoAction(Motions::stance current, Motions::action a)
                {
                        std::map<Motions::action, Motions::Action_Transition> _actions = Motions::A::create_actions(); // move this out of here!
                        Motions::Action_Transition at = _actions[a];

                        Motions::Stance_Path p;
                        p.add_stance(current, 1);

                        p = decide_stance(current, at._s, p); //I would use GoToStance however I need the path step offset
                        memcpy(_stance_action, p._stances, sizeof(p._stances));

                        _stance_action[p.offset()] = Motions::NUM_OF_STANCES + a;

                        _num_of_stance_actions = static_cast<int8_t>(p.offset()+1);
                        _stance_action_mask = true;

                        return p.cost() + at._cost;
                }

#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
                /// string constructor (see from_string() below)
                MOTION_Commands(const std::string &str) { from_string(str); }

                /**
                 * set head and/or body stiffness, and/or start a motion
                 * through a string of the form
                 *
                 * [0|1][,0|1][[,]motion_command]
                 */
                void from_string(const std::string &str)
                {
                        set_ignore_chain_mask(false);
                        
                        using namespace Motions;
                        const char *s = str.c_str();
                        bool want_head_stiffness = isdigit(*s);

                        if (want_head_stiffness)
                        {
                                set_head_stiffness_mask();
                                set_head_stiffness(unsigned(atoi(s)));
                                while (isdigit(*s)) s++;
                        }
                        else set_head_stiffness_mask(false);
                        if (*s == ',' && isdigit(*++s)) // check body stiffness?
                        {
                                set_body_stiffness_mask();
                                set_body_stiffness(unsigned(atoi(s)));
                                while (isdigit(*s)) s++;
                        }
                        else set_body_stiffness_mask(false);

                        if(*s != ',')
                        {
                                set_stance_action_mask(false);
                                return;
                        }
                        const char *ss = s;
                        ss++;
                        const char *p = strchr(ss, ',');
                        set_stance_action_mask(p != NULL);
                        std::string str2(ss);
                        str2 = str2.substr(0, unsigned(p-ss+1));
                        
                        stance s1;
                        MY_CHECK_ENUM(str2.c_str(), s1, Standby_stance);
                        else MY_CHECK_ENUM(str2.c_str(), s1, Kneeling_stance);
                        else MY_CHECK_ENUM(str2.c_str(), s1, Standing_stance);
                        else MY_CHECK_ENUM(str2.c_str(), s1, GoalieSaveLeft_stance);
                        else MY_CHECK_ENUM(str2.c_str(), s1, GoalieSaveRight_stance);
                        else MY_CHECK_ENUM(str2.c_str(), s1, FallenForward_stance);
                        else MY_CHECK_ENUM(str2.c_str(), s1, FallenBack_stance);
                        else s1 = NUM_OF_STANCES;

                        while (*(++s) != ',') ;

                        action a2;
                        MY_CHECK_ENUM(s, a2, Kneeling_wave);
			else MY_CHECK_ENUM(s, a2, Kneeling_quickwave);
			else MY_CHECK_ENUM(s, a2, Kneeling_flagwave);
			else MY_CHECK_ENUM(s, a2, Standing_leftkick);
			else MY_CHECK_ENUM(s, a2, Standing_rightkick);
			else MY_CHECK_ENUM(s, a2, Standing_leftpass);
			else MY_CHECK_ENUM(s, a2, Standing_rightpass);
			else MY_CHECK_ENUM(s, a2, Standing_wave);
                        else a2 = NUM_OF_ACTIONS;

			if(a2 != NUM_OF_ACTIONS)   
			{
				DoAction(s1, a2);
				return;
			}

                        stance s2;
                        MY_CHECK_ENUM(s, s2, Standby_stance);
                        else MY_CHECK_ENUM(s, s2, Kneeling_stance);
                        else MY_CHECK_ENUM(s, s2, Standing_stance);
                        else MY_CHECK_ENUM(s, s2, GoalieSaveLeft_stance);
                        else MY_CHECK_ENUM(s, s2, GoalieSaveRight_stance);
                        else MY_CHECK_ENUM(s, s2, FallenForward_stance);
                        else MY_CHECK_ENUM(s, s2, FallenBack_stance);
                        else s2 = NUM_OF_STANCES;

                        GoToStance(s1, s2);
                }

#pragma clang diagnostic ignored "-Wcovered-switch-default"
#pragma clang diagnostic ignored "-Wunreachable-code"

                /**
                 * return the head and body stiffness as a boolean
                 */
                std::string description() const
                {
                        /*
                        using namespace Motions;
                        std::stringstream ss;
                        bool need_comma = false;
                        if (head_stiffness_mask())
                        {
                                ss << head_stiffness();
                                need_comma = true;
                        }
                        if (body_stiffness_mask())
                        {
                                ss << "," << body_stiffness();
                                need_comma = true;
                        }
                        if (motion_player_mask())
                        {
                                if (need_comma) ss << ",";
                                switch (motion_player())
                                {
                                        STREAM_ENUM_CASE(ss, right_goalie_block);
                                        STREAM_ENUM_CASE(ss, finish_right_goalie_block);
                                        STREAM_ENUM_CASE(ss, left_goalie_block);
                                        STREAM_ENUM_CASE(ss, finish_left_goalie_block);
                                        STREAM_ENUM_CASE(ss, get_up_from_back_lie_down);
                                        STREAM_ENUM_CASE(ss, get_up_from_front_lie_down);
                                        STREAM_ENUM_CASE(ss, get_up_from_kneel);
                                        STREAM_ENUM_CASE(ss, get_up_from_sit);
                                        STREAM_ENUM_CASE(ss, goalie_stand_pose);
                                        STREAM_ENUM_CASE(ss, hip_turn_left_kick);
                                        STREAM_ENUM_CASE(ss, hip_turn_right_kick);
                                        STREAM_ENUM_CASE(ss, kneel);
                                        STREAM_ENUM_CASE(ss, left_kick);
                                        STREAM_ENUM_CASE(ss, left_ninja_kick);
                                        STREAM_ENUM_CASE(ss, pass_ball_left);
                                        STREAM_ENUM_CASE(ss, pass_ball_right);
                                        STREAM_ENUM_CASE(ss, right_kick);
                                        STREAM_ENUM_CASE(ss, right_ninja_kick);
                                        STREAM_ENUM_CASE(ss, side_to_lie_down);
                                        STREAM_ENUM_CASE(ss, sit_from_front_lie_down);
                                        STREAM_ENUM_CASE(ss, sit_from_back_lie_down);
                                        STREAM_ENUM_CASE(ss, sit_from_kneel_or_stand);
                                        STREAM_ENUM_CASE(ss, dcm_from_getup);
                                        STREAM_ENUM_CASE(ss, OFF);
                                        default: break;
                                }
                        }
                        return ss.str();*/
                        return std::string("");
                }
#endif // WHITEBOARD_POSTER_STRING_CONVERSION
       };
}

#pragma clang diagnostic pop

#endif //MOTION_Commands_DEFINED
