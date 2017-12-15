/**                                                                     
 *  /file SENSORS_LedsSensors.h
 *                                                                      
 *  Created by Carl Lusty in 2013.                                      
 *  Copyright (c) 2013 Carl Lusty                                       
 *  All rights reserved.                                                
 */                                                                     

                                                                        
#ifndef SENSORS_LedsSensors_DEFINED
#define SENSORS_LedsSensors_DEFINED


#include <gu_util.h>
#include <assert.h>
#include "wb_sensors_leds.h" 

namespace guWhiteboard                                                  
{
        enum LEDColour
        {
                Off = -1,
                Red = 0,
                Green,
                Blue,
                Yellow,
                NUMBER_OF_LEDS_COLOURS
        };
        
        enum LEDGroups
        {
                AllLEDs = 0,        //Only off is supported regardless of what is passed in
                Ears,               //Number of leds to turn on is the value
                LEar,               //All others have the desired colour as the passed value
                REar,
                Eyes,
                LEye,
                REye,
                Chest,
                Feet,
                LFoot,
                RFoot,
                NUMBER_OF_LEDS_GROUPS
        };
        
        namespace LEDs
        {
                enum LEDCode
                {
                        LeftEar1 = 0,
                        LeftEar2,
                        LeftEar3,
                        LeftEar4,
                        LeftEar5,
                        LeftEar6,
                        LeftEar7,
                        LeftEar8,
                        LeftEar9,
                        LeftEar10,
                        RightEar1,
                        RightEar2,
                        RightEar3,
                        RightEar4,
                        RightEar5,
                        RightEar6,
                        RightEar7,
                        RightEar8,
                        RightEar9,
                        RightEar10,
                        LeftEyeRed1,
                        LeftEyeRed2,
                        LeftEyeRed3,
                        LeftEyeRed4,
                        LeftEyeRed5,
                        LeftEyeRed6,
                        LeftEyeRed7,
                        LeftEyeRed8,
                        LeftEyeGreen1,
                        LeftEyeGreen2,
                        LeftEyeGreen3,
                        LeftEyeGreen4,
                        LeftEyeGreen5,
                        LeftEyeGreen6,
                        LeftEyeGreen7,
                        LeftEyeGreen8,
                        LeftEyeBlue1,
                        LeftEyeBlue2,
                        LeftEyeBlue3,
                        LeftEyeBlue4,
                        LeftEyeBlue5,
                        LeftEyeBlue6,
                        LeftEyeBlue7,
                        LeftEyeBlue8,
                        RightEyeRed1,
                        RightEyeRed2,
                        RightEyeRed3,
                        RightEyeRed4,
                        RightEyeRed5,
                        RightEyeRed6,
                        RightEyeRed7,
                        RightEyeRed8,
                        RightEyeGreen1,
                        RightEyeGreen2,
                        RightEyeGreen3,
                        RightEyeGreen4,
                        RightEyeGreen5,
                        RightEyeGreen6,
                        RightEyeGreen7,
                        RightEyeGreen8,
                        RightEyeBlue1,
                        RightEyeBlue2,
                        RightEyeBlue3,
                        RightEyeBlue4,
                        RightEyeBlue5,
                        RightEyeBlue6,
                        RightEyeBlue7,
                        RightEyeBlue8,
                        ChestRed,
                        ChestGreen,
                        ChestBlue,
                        LeftFootRed,
                        LeftFootGreen,
                        LeftFootBlue,
                        RightFootRed,
                        RightFootGreen,
                        RightFootBlue,
                        NUMBER_OF_LEDS
                };
                
#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
                static const char *ledNames[NUMBER_OF_LEDS] =
                {
                        "LeftEar1",
                        "LeftEar2",
                        "LeftEar3",
                        "LeftEar4",
                        "LeftEar5",
                        "LeftEar6",
                        "LeftEar7",
                        "LeftEar8",
                        "LeftEar9",
                        "LeftEar10",
                        "RightEar1",
                        "RightEar2",
                        "RightEar3",
                        "RightEar4",
                        "RightEar5",
                        "RightEar6",
                        "RightEar7",
                        "RightEar8",
                        "RightEar9",
                        "RightEar10",
                        "LeftEyeRed1",
                        "LeftEyeRed2",
                        "LeftEyeRed3",
                        "LeftEyeRed4",
                        "LeftEyeRed5",
                        "LeftEyeRed6",
                        "LeftEyeRed7",
                        "LeftEyeRed8",
                        "LeftEyeGreen1",
                        "LeftEyeGreen2",
                        "LeftEyeGreen3",
                        "LeftEyeGreen4",
                        "LeftEyeGreen5",
                        "LeftEyeGreen6",
                        "LeftEyeGreen7",
                        "LeftEyeGreen8",
                        "LeftEyeBlue1",
                        "LeftEyeBlue2",
                        "LeftEyeBlue3",
                        "LeftEyeBlue4",
                        "LeftEyeBlue5",
                        "LeftEyeBlue6",
                        "LeftEyeBlue7",
                        "LeftEyeBlue8",
                        "RightEyeRed1",
                        "RightEyeRed2",
                        "RightEyeRed3",
                        "RightEyeRed4",
                        "RightEyeRed5",
                        "RightEyeRed6",
                        "RightEyeRed7",
                        "RightEyeRed8",
                        "RightEyeGreen1",
                        "RightEyeGreen2",
                        "RightEyeGreen3",
                        "RightEyeGreen4",
                        "RightEyeGreen5",
                        "RightEyeGreen6",
                        "RightEyeGreen7",
                        "RightEyeGreen8",
                        "RightEyeBlue1",
                        "RightEyeBlue2",
                        "RightEyeBlue3",
                        "RightEyeBlue4",
                        "RightEyeBlue5",
                        "RightEyeBlue6",
                        "RightEyeBlue7",
                        "RightEyeBlue8",
                        "ChestRed",
                        "ChestGreen",
                        "ChestBlue",
                        "LeftFootRed",
                        "LeftFootGreen",
                        "LeftFootBlue",
                        "RightFootRed",
                        "RightFootGreen",
                        "RightFootBlue",
                };
#endif
        }
        
        class SENSORS_LedsSensors : public wb_sensors_leds
        {
                
        public:
                SENSORS_LedsSensors()
                {
                        LEDsGroupChange(AllLEDs, Off);
                }
                
                void LEDsGroupChange(LEDGroups target_group, LEDColour target_colour_or_numOfEars)
                {
                        switch (target_group)
                        {
                                case AllLEDs: //Only supporting off command
                                {
                                        for(int i = LEDs::LeftEar1; i < LEDs::NUMBER_OF_LEDS; i++)
                                                changeLed(static_cast<LEDs::LEDCode>(i), false);
                                        break;
                                }
                                case Ears:
                                {
                                        for(int i = LEDs::LeftEar1; i <= LEDs::LeftEar10; i++)
                                                i-LEDs::LeftEar1 < target_colour_or_numOfEars ? changeLed(static_cast<LEDs::LEDCode>(i), true) : changeLed(static_cast<LEDs::LEDCode>(i), false);
                                        for(int i = LEDs::RightEar1; i <= LEDs::RightEar10; i++)
                                                i-LEDs::RightEar1 < target_colour_or_numOfEars ? changeLed(static_cast<LEDs::LEDCode>(i), true) : changeLed(static_cast<LEDs::LEDCode>(i), false);
                                        break;
                                }
                                case LEar:
                                {
                                        for(int i = LEDs::LeftEar1; i <= LEDs::LeftEar10; i++)
                                                i-LEDs::LeftEar1 < target_colour_or_numOfEars ? changeLed(static_cast<LEDs::LEDCode>(i), true) : changeLed(static_cast<LEDs::LEDCode>(i), false);
                                        break;
                                }
                                case REar:
                                {
                                        for(int i = LEDs::RightEar1; i <= LEDs::RightEar10; i++)
                                                i-LEDs::RightEar1 < target_colour_or_numOfEars ? changeLed(static_cast<LEDs::LEDCode>(i), true) : changeLed(static_cast<LEDs::LEDCode>(i), false);
                                        break;
                                }
                                case Eyes:
                                {
                                        for(int i = LEDs::LeftEyeRed1; i <= LEDs::LeftEyeRed8; i++)
                                        {
                                                LEDs::LEDCode led[3] = {LEDs::LEDCode(i), LEDs::LEDCode(i+(LEDs::LeftEyeGreen1-LEDs::LeftEyeRed1)), LEDs::LEDCode(i+(LEDs::LeftEyeBlue1-LEDs::LeftEyeRed1))};
                                                changeLedColour(led, target_colour_or_numOfEars);
                                        }
                                        for(int i = LEDs::RightEyeRed1; i <= LEDs::RightEyeRed8; i++)
                                        {
                                                LEDs::LEDCode led[3] = {LEDs::LEDCode(i), LEDs::LEDCode(i+(LEDs::RightEyeGreen1-LEDs::RightEyeRed1)), LEDs::LEDCode(i+(LEDs::RightEyeBlue1-LEDs::RightEyeRed1))};
                                                changeLedColour(led, target_colour_or_numOfEars);
                                        }
                                        break;
                                }
                                case LEye:
                                {
                                        for(int i = LEDs::LeftEyeRed1; i <= LEDs::LeftEyeRed8; i++)
                                        {
                                                LEDs::LEDCode led[3] = {LEDs::LEDCode(i), LEDs::LEDCode(i+(LEDs::LeftEyeGreen1-LEDs::LeftEyeRed1)), LEDs::LEDCode(i+(LEDs::LeftEyeBlue1-LEDs::LeftEyeRed1))};
                                                changeLedColour(led, target_colour_or_numOfEars);
                                        }
                                        break;
                                }
                                case REye:
                                {
                                        for(int i = LEDs::RightEyeRed1; i <= LEDs::RightEyeRed8; i++)
                                        {
                                                LEDs::LEDCode led[3] = {LEDs::LEDCode(i), LEDs::LEDCode(i+(LEDs::RightEyeGreen1-LEDs::RightEyeRed1)), LEDs::LEDCode(i+(LEDs::RightEyeBlue1-LEDs::RightEyeRed1))};
                                                changeLedColour(led, target_colour_or_numOfEars);
                                        }
                                        break;
                                }
                                case Chest:
                                {
                                        LEDs::LEDCode led[3] = {LEDs::ChestRed, LEDs::ChestGreen, LEDs::ChestBlue};
                                        changeLedColour(led, target_colour_or_numOfEars);
                                        break;
                                }
                                case Feet:
                                {
                                        LEDs::LEDCode led[3] = {LEDs::LeftFootRed, LEDs::LeftFootGreen, LEDs::LeftFootBlue};
                                        changeLedColour(led, target_colour_or_numOfEars);
                                        LEDs::LEDCode led2[3] = {LEDs::RightFootRed, LEDs::RightFootGreen, LEDs::RightFootBlue};
                                        changeLedColour(led2, target_colour_or_numOfEars);
                                        break;
                                }
                                case LFoot:
                                {
                                        LEDs::LEDCode led[3] = {LEDs::LeftFootRed, LEDs::LeftFootGreen, LEDs::LeftFootBlue};
                                        changeLedColour(led, target_colour_or_numOfEars);
                                        break;
                                }
                                case RFoot:
                                {
                                        LEDs::LEDCode led[3] = {LEDs::RightFootRed, LEDs::RightFootGreen, LEDs::RightFootBlue};
                                        changeLedColour(led, target_colour_or_numOfEars);
                                        break;
                                }
                                case NUMBER_OF_LEDS_GROUPS:
                                        break;
                        }
                }
                
        private:
                void changeLedColour(LEDs::LEDCode led[3], LEDColour colour)
                {
                        //led[0] R, led[1] G, led[2] B
                        switch (colour)
                        {
                                case Off:
                                        changeLed(led[0], false);
                                        changeLed(led[1], false);
                                        changeLed(led[2], false);
                                        break;
                                case Red:
                                        changeLed(led[0], true);
                                        changeLed(led[1], false);
                                        changeLed(led[2], false);
                                        break;
                                case Green:
                                        changeLed(led[0], false);
                                        changeLed(led[1], true);
                                        changeLed(led[2], false);
                                        break;
                                case Blue:
                                        changeLed(led[0], false);
                                        changeLed(led[1], false);
                                        changeLed(led[2], true);
                                        break;
                                case Yellow:
                                        changeLed(led[0], static_cast<u_int8_t>(100));
                                        changeLed(led[1], static_cast<u_int8_t>(25));
                                        changeLed(led[2], static_cast<u_int8_t>(0));
                                        break;
                                case NUMBER_OF_LEDS_COLOURS:
                                        break;
                        }
                }
                
                void changeLed(LEDs::LEDCode led, bool state)
                {
                        if(led <= LEDs::RightEar10)
                                _ears[led] = state ? true : false;
                        else
                                _leds[led-LEDs::RightEar10] = state ? 100 : 0;
                }
                
                void changeLed(LEDs::LEDCode led, u_int8_t level) //level == percentage, 100 == MAX
                {
                        assert(led > LEDs::RightEar10);

                        _leds[led-LEDs::RightEar10] = level;
                }
        };
}

#endif //SENSORS_LedsSensors_DEFINED
