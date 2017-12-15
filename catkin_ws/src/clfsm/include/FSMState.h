/*
 *  FSMState.h
 *  
 *  Created by René Hexel on 23/09/11.
 *  Copyright (c) 2011 Rene Hexel.
 *  All rights reserved.
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
#ifndef gufsm_FSMState_h
#define gufsm_FSMState_h

#include <string>
#include "FSMActivity.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"

namespace FSM
{
        class Transition;
        
        typedef std::vector<Transition *> TransitionVector;
        typedef TransitionVector::iterator TransitionIterator;
        
        class State
        {
                Activity _activity;     ///< the activity represented by this state
                std::string _name;      ///< name of the state
                
                /** list of transitions leading away from this state */
                TransitionVector _transitions;

                int _stateID;           ///< state number within machine
        public:
                State(int stateID = 0, const std::string name = ""):
                _activity(), _name(name), _transitions(), _stateID(stateID) {}

                virtual ~State() {}

                /** get the numerical state ID */
                int stateID() const { return _stateID; }
                
                /** set the numerical state ID */
                void setStateID(int i) { _stateID = i; }
                
                /** get the state's name */
                const std::string &name() const { return _name; }
                
                /** set the state's name */
                void setName(const std::string &name) { _name = name; }
                
                /** get the state's activity */
                Activity &activity() { return _activity; }
                
                /** get the state's activity as a const reference */
                const Activity &activity() const { return _activity; }

                /** get that state's transitions */
                TransitionVector &transitions() { return _transitions; }
                
                /** get that state's transitions as a const reference */
                const TransitionVector &transitions() const { return _transitions; }

                /** add a transition */
                void addTransition(Transition *t) { _transitions.push_back(t); }
                /**
                 * printable state  description
                 */
                virtual std::string description() const;
        };
}

#pragma clang diagnostic pop

#endif
