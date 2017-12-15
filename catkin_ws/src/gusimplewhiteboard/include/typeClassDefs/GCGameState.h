/*
 *  /GCGameState.h
 *  gusimplewhiteboard / clfsm
 *
 *  Created by Rene Hexel on 25/03/13.
 *  Copyright (c) 2013 Rene Hexel. All rights reserved.
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
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
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
#ifndef GCGameState_DEFINED
#define GCGameState_DEFINED

#include <cstdlib>
#include <stdint.h>
#include <sys/types.h>
#include <sstream>
#include <gu_util.h>

#define SPL_NUM_PLAYERS 5
#define SPL_NUM_TEAMS 2

#define DEFAULT_PLAYER_NUMBER 2

#define kGSTeamBlue "TeamBlue"
#define kGSTeamRed "TeamRed"

#define kGSFirstHalf "FirstHalf"
#define kGSSecondHalf "SecondHalf"

#define kGSNormalGame "NormalGame"
#define kGSPenaltyShots "PenaltyShots"
#define kGSOverTime "OverTime"

#define kGSInitialReceived "InitialReceived"
#define kGSReadyReceived "ReadyReceived"
#define kGSSetReceived "SetReceived"
#define kGSPlayingReceived "PlayingReceived"
#define kGSFinishedReceived "FinishedReceived"

#define kGSInitial "GSInitial"
#define kGSReady "GSReady"
#define kGSSet  "GSSet"
#define kGSPlaying  "GSPlaying"
#define kGSFinished  "GSFinished"

#define kGSNoPenalty "NoPenalty"
#define kGSBallHolding "BallHolding"
#define kGSPlayerPushing "PlayerPushing"
#define kGSObstruction "Obstruction"
#define kGSInactivePlayer "InactivePlayer"
#define kGSIllegalDefender "IllegalDefender"
#define kGSLeavingTheField "LeavingTheField"
#define kGSPlayingWithHands "PlayingWithHands"
#define kGSRequestForPickup "RequestForPickup"


#define kNoGSsignal "NoGSsignal"
#define kGSOurGoalSignalPushed "GSOurGoalSignalPushed"
#define kGSTheirGoalSignalPushed "GSTheirGoalSignalPushed"
#define kGSBlueKickOffSignalPushed "GSBlueKickOffSignalPushed"
#define kGSRedKickOffSignalPushed "GSRedKickOffSignalPushed"

namespace guWhiteboard
{
	enum TeamColours
	{  TeamBlue=0, TeamRed=1, SentinelTeamColours=SPL_NUM_TEAMS };

	enum GameHalf
	{  SecondHalf, FirstHalf };

	enum GameFormat
	{  NormalGame, PenaltyShots, OverTime };

	enum BallOut
	{  OutByBlue, OutByRed };

	enum GameState {  Initial, Ready, Set, Playing, Finished };

	enum GameControllerCommand
	{  InitialReceived, ReadyReceived, SetReceived, PlayingReceived, FinishedReceived };

	enum PenaltyFormat
	{ NoPenalty, BallHolding, PlayerPushing, Obstruction, InactivePlayer, IllegalDefender, LeavingTheField, PlayingWithHands, RequestForPickup, ManualButtonPenalty };

	enum GameContollerSignal
	{  NoGSsignal, GSOurGoalSignalPushed, GSTheirGoalSignalPushed, GSBlueKickOffSignalPushed, GSRedKickOffSignalPushed };

        /**
	 * Class to annoucne to out class-oriented whiteboard what we got in GSreceiver
         */

        class GCGameState
        {
	   private:
		 uint8_t  _whatPenaltyFromUsInGSgameController[SPL_NUM_PLAYERS];
		 uint8_t _whatPenaltyFromThemInGSgameController[SPL_NUM_PLAYERS];

		 int16_t _score [SentinelTeamColours];
		 bool _dropInTeam;

                PROPERTY(GameHalf, theGSHalf )  //  GS half
                PROPERTY(GameFormat, theGSGameformat )  //  GS game format
                PROPERTY(GameState, theGSGameState )  
		//  GS GameControllerCommand
                PROPERTY(GameControllerCommand, theGSGameControllerCommand )  
		//  GS team that has kickoff
                PROPERTY(TeamColours, theGSteamThatHasKickOf )  
		//  GS team that has ball after it went out
                PROPERTY(TeamColours, theGSteamCausedlastDropIn )  
                PROPERTY(int16_t, theSecondReminingInHalf )  
		//  GS team that has ball after it went out
                PROPERTY(TeamColours, theGScolourWePlayWith )  

        public:
            /** designated constructor */
            GCGameState(GameHalf theGSHalf = FirstHalf, 
                                       GameFormat theGSGameformat = NormalGame,
				       GameState theGSGameState = Initial,
				       TeamColours theGSteamThatHasKickOf = TeamBlue,
				       TeamColours theGSteamCausedlastDropIn = TeamBlue,
				       int16_t theSecondReminingInHalf = TeamBlue ,
				       TeamColours theGScolourWePlayWith = TeamBlue
				       ):
                                      _theGSHalf(theGSHalf), 
                                      _theGSGameformat(theGSGameformat),
                                      _theGSGameState(theGSGameState),
                                      _theGSteamThatHasKickOf(theGSteamThatHasKickOf),
                                      _theGSteamCausedlastDropIn(theGSteamCausedlastDropIn),
                                      _theSecondReminingInHalf(theSecondReminingInHalf),
                                      _theGScolourWePlayWith(theGScolourWePlayWith)
                                        {
			for (int i=0; i< SPL_NUM_PLAYERS; i++)
			{
				_whatPenaltyFromUsInGSgameController[i]=NoPenalty;
				_whatPenaltyFromThemInGSgameController[i]=NoPenalty;
			}
			_dropInTeam=false;
			for (int i=TeamBlue; i< SentinelTeamColours; i++) _score[i]=0;
                }

            /** string constructor */
            GCGameState(const std::string &names) { from_string(names); }

            /** copy constructor */
            GCGameState(const GCGameState &other):
                      _theGSHalf(other._theGSHalf),
                      _theGSGameformat(other._theGSGameformat),
                       _theGSGameState(other._theGSGameState),
                       _theGSteamThatHasKickOf(other._theGSteamThatHasKickOf),
                       _theGSteamCausedlastDropIn(other._theGSteamCausedlastDropIn),
                       _theSecondReminingInHalf(other._theSecondReminingInHalf),
                       _theGScolourWePlayWith(other._theGScolourWePlayWith)
                       { for (int i=0; i< SPL_NUM_PLAYERS; i++)
			{
				_whatPenaltyFromUsInGSgameController[i]=other._whatPenaltyFromUsInGSgameController[i];
				_whatPenaltyFromThemInGSgameController[i]=other._whatPenaltyFromThemInGSgameController[i];
			}
			_dropInTeam=other._dropInTeam;
			for (int i=0; i< SPL_NUM_TEAMS; i++) _score[i]=other._score[i];
                      }

            const GCGameState &operator=(const GCGameState &other) { memcpy(this, &other, sizeof(*this)); return *this; }

            /** Set the Penalty vectors */
	    void setPenaltyVectors(const PenaltyFormat thePenaltyFromUsInGSgameController[SPL_NUM_PLAYERS],
		                    const PenaltyFormat thePenaltyFromThemInGSgameController[SPL_NUM_PLAYERS])
			    {
				    for (int i=0; i<SPL_NUM_PLAYERS; i++)
				    {
						_whatPenaltyFromUsInGSgameController[i]=thePenaltyFromUsInGSgameController[i];
						_whatPenaltyFromThemInGSgameController[i]=thePenaltyFromThemInGSgameController[i];
				    }
			    }

            /** Set the score */
	    void setScore(int16_t goalsByBlue, int16_t goalsByRed)
		{  _score[TeamBlue] = goalsByBlue;
		   _score[TeamRed] = goalsByRed;
		}

		int16_t getScoreBlue() { return _score[TeamBlue];}
		int16_t getScoreRed() { return _score[TeamRed];}

		int16_t getOurScore() { return _score[_theGScolourWePlayWith];}
		int16_t getTheirScore() { 
				if (TeamBlue== _theGScolourWePlayWith ) return getScoreRed();
				else return getScoreBlue(); }

		PenaltyFormat myPenaltyIs (int PlayerNumber)
		{	// the idnex in the structure starts at 0, numebrs on robot's backs start at 1
			  if ((0<PlayerNumber) && (PlayerNumber<=SPL_NUM_PLAYERS ))
					return static_cast<PenaltyFormat>(_whatPenaltyFromUsInGSgameController[PlayerNumber-1]);
			  else return NoPenalty;
		}

		bool amIPenalized(int PlayerNumber)
		{

			if (NoPenalty==myPenaltyIs(PlayerNumber))
			   return false;
			else return true;
		}

		void setMyPenalty(int PlayerNumber, PenaltyFormat thePenalty)
				{
					//if (ManualButtonPenalty==thePenalty )
					   _whatPenaltyFromUsInGSgameController[PlayerNumber-1]=thePenalty;
					//else 
					 //   _whatPenaltyFromUsInGSgameController[PlayerNumber-1]=NoPenalty;
				}




            /** convert to a string */
            std::string description()
            {
                std::ostringstream ss;
	        if  ( TeamBlue == theGSteamThatHasKickOf() ) ss << kGSTeamBlue<<","; else ss << kGSTeamRed<<",";

	        if  ( FirstHalf == theGSHalf() ) ss << kGSFirstHalf<<","; else ss << kGSSecondHalf<<",";

	        if  ( NormalGame == theGSGameformat() ) ss << kGSNormalGame<<","; else ss << kGSPenaltyShots<<",";

		switch(int (_theGSGameState))
		{ case Initial: ss << kGSInitial<<",";
			break;
		  case Ready: ss << kGSReady<<",";
			break;
		  case Set: ss << kGSSet<<",";
			break;
		  case Playing: ss << kGSPlaying<<",";
			break;
		  case Finished: ss << kGSFinished<<",";
			break;
	          default: ss << _theGSGameState <<",";
		}


                for (int i=0; i< SPL_NUM_PLAYERS; i++)
		switch (_whatPenaltyFromUsInGSgameController[i] )
		{ case NoPenalty : ss << kGSNoPenalty<<",";
			break;
		  case BallHolding : ss << kGSBallHolding<<",";
			break;
		  case PlayerPushing : ss << kGSPlayerPushing<<",";
			break;
		  case Obstruction : ss << kGSObstruction<<",";
			break;
		  case InactivePlayer : ss << kGSInactivePlayer<<",";
			break;
		  case IllegalDefender : ss << kGSIllegalDefender<<",";
			break;
		  case LeavingTheField : ss << kGSLeavingTheField<<",";
			break;
		  case PlayingWithHands : ss << kGSPlayingWithHands<<",";
			break;
		  case RequestForPickup : ss << kGSRequestForPickup<<",";
			break;
		}

		ss << getScoreBlue() << "," <<  getScoreRed() << ",";



		/*
		switch (theGSGameControllerCommand() )
		{ case InitialReceived : ss << kGSInitialReceived<<",";
			break;
		  case ReadyReceived : ss << kGSReadyReceived<<",";
			break;
		  case SetReceived : ss << kGSSetReceived<<",";
			break;
		  case PlayingReceived : ss << kGSPlayingReceived<<",";
			break;
		  case FinishedReceived : ss << kGSFinishedReceived<<",";
			break;
		}

		switch (theGSGameContollerSignal() )
		{ case NoGSsignal : ss << kNoGSsignal<<",";
			break;
		  case GSOurGoalSignalPushed : ss << kGSOurGoalSignalPushed<<",";
			break;
		  case GSTheirGoalSignalPushed : ss << kGSTheirGoalSignalPushed<<",";
			break;
		  case  GSBlueKickOffSignalPushed: ss << kGSBlueKickOffSignalPushed<<",";
			break;
		  case GSRedKickOffSignalPushed : ss << kGSRedKickOffSignalPushed<<",";
			break;
		}
		*/

                return ss.str();
            }

            /** TODO: convert from a string */
            void from_string(const std::string &str)
            {
                std::istringstream iss(str);
                std::string token;
                getline(iss, token, ',');
            }
        };
}


#endif // GCGameState_DEFINED
