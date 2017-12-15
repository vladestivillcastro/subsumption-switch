/*
 *  WhiteboardConstants.h
 *  guboostwhiteboard
 *
 *  Created by Rene Hexel on 1/05/11.
 *  Copyright 2011 Rene Hexel. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgement:
 *
 *        This product includes software developed by Rene Hexel.
 *
 * 4. Neither the name of the author nor the names of contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * -----------------------------------------------------------------------
 * This program is free software; you can redistribute it and/or
 * modify it under the above terms or under the terms of the GNU
 * General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see http://www.gnu.org/licenses/
 * or write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */
#ifndef _WHITEBOARD_CONSTANTS_H_
#define _WHITEBOARD_CONSTANTS_H_

namespace guWhiteboard
{
	/** Soap callback key for message subscriber method */
	extern const char *kOnWhiteboard;
	
	/** Hello World message name */
	extern const char *kHelloWBMsg;
       
	/** Speech message (string) */
	extern const char *kSay;
        
	/** Queuing speech message (string) */
	extern const char *kQSay;
        
	/** Debug speech message (string) -- only use for debugging,
         *  as this will only work if "SpeechOutput" "On" was sent
         *  previously
         */
	extern const char *kSpeechWBMsg;
        
	/** Queuing debug speech message (string) -- only use for debugging,
         *  as this will only work if "SpeechOutput" "On" was sent
         *  previously
         */
	extern const char *kQSpeech;
        
        /** Whiteboard message for conditional speech output.
         *  Content (string) should be "On" to turn on speech or "Off"
         *  to turn off speech (will not affect the "Say" command).
         */
        extern const char *kSpeechOutput;
        
        
	/*!
	 * This variable is 1 if we are playng as RED and
	 * is 0 if we are playng as BLUE
	 */
	extern const char *kRobocup_ourTeam;

	/*!
	 * This variable is 1 if we it is our kickoff 
	 * is 0 if we are NOT having the kickoff.
	 * Regardless of what team we are playing kRobocup_ourTeam==kRobocup_kickOffTeam
	 * tests if we are kicking off
	 * 
	 */
	extern const char *kRobocup_kickOffTeam;
	

	/** Whiteboard message (string) for updating all proofs */
	extern const char *kUpdateAllProofs;



	/** Whiteboard message (string) for updating proofs for a given theory */
	extern const char *kUpdateAllProofsForTheory;

	/** Whiteboard message (string) for updating a proof for a given output */
	extern const char *kUpdateProof;
	
	/** Whiteboard message (string) for turn on modules */
	extern const char *kOn;
	
	/** Whiteboard message (string) for turn off modules */
	extern const char *kOff;

	/**Differential Robot Whiteboard message to turn a motor on 
	 * Content (Array): Motor(left = 0, right = 1), power %
	 * */
	extern const char *kDifferentialMotorPower;

	/**Differential Robot Whiteboard message to stop a touch sensor
	 * Content (Int): Id of touch sensor for NXT(1, 2, 3 or 4)
	 * */
	extern const char *kDifferentialStopTouch;

	/**Differential Robot Whiteboard message to start a touch sensor
	 * Content (Int): Id of touch sensor for NXT(1, 2, 3 or 4)
	 * */
	extern const char *kDifferentialStartTouch;

	/**Differential Robot Whiteboard message providing the value of a touch sensor*/
	extern const char *kDifferentialTouchValue;

	/**Differential Robot Whiteboard message to start a distance sensor
	 * Content (Int): Id of distance sensor for NXT Sonar sensors(1, 2, 3 or 4)
	 * */
	extern const char *kDifferentialStartDistance;

	/**Differential Robot Whiteboard message to stop a distance sensor
	 * Content (Int): Id of distance sensor for NXT Sonar sensors(1, 2, 3 or 4)
	 * */
	extern const char *kDifferentialStopDistance;

	/**Differential Robot Whiteboard message providing the value of a distance sensor*/
	extern const char *kDifferentialDistanceValue;

	/**Differential Robot Whiteboard message to start a light sensor
	 * Content (Int): Id of light sensor for NXT sensors(1, 2, 3 or 4)
	 * */
	extern const char *kDifferentialStartLight;

	/**Differential Robot Whiteboard message to stop a light sensor
	 * Content (Int): Id of light sensor for NXT sensors(1, 2, 3 or 4)
	 * */
	extern const char *kDifferentialStopLight;

	/**Differential Robot Whiteboard message providing the value of a light sensor*/
	extern const char *kDifferentialLightValue;

	/**Differential Robot Whiteboard message to turn on a led*/
	extern const char *kDifferentialLedOn;

	/**Differential Robot Whiteboard message to turn off a led*/
	extern const char *kDifferentialLedOff;

	/**Differential Robot Whiteboard message to start playing a sound
	 * Content (Int): Duration in milisecond
	 * */
	extern const char *kDifferentialPlaySound;

	/**Differential Robot Whiteboard message to stop playing a sound
	 * Content(Any) N/A
	 * */
	extern const char *kDifferentialStopSound;

	/**Differential Robot Whiteboard message to start a rotation sensor
	 * Content (Int): Motor(left = 0, right = 1)
	 * */
	extern const char *kDifferentialStartRotationSensor;

	/**Differential Robot Whiteboard message to stop a rotation sensor
	 * Content (Int): Motor(left = 0, right = 1)
	 * */
	extern const char *kDifferentialStopRotationSensor;

	/**Differential Robot Whiteboard message providing the value of a rotation sensor*/
		extern const char *kDifferentialRotationValue;

	//ID's for motor's in differential robots
	//renamed so it doe s not class with OO_Whiteboard types
	enum OLDDifferentialMotor {
		OLD_LEFT_MOTOR_DIFFERENTIAL = 0,
		OLD_RIGHT_MOTOR_DIFFERENTIAL = 1,
		OLD_NXT_MOTOR3 = 2 //constant for the third motor on nxt's
	};

}

#endif // _WHITEBOARD_CONSTANTS_H_
