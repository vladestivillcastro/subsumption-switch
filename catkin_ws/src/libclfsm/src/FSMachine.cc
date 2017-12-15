/*
 *  FSMachine.cc
 *  
 *  Created by René Hexel on 23/09/11.
 *  Copyright (c) 2011, 2013, 2014 Rene Hexel.
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
#include <sstream>
#include <cassert>
#include "FSMachine.h"
#include "FSMState.h"
#include "FSMTransition.h"
#include "FSMExpression.h"
#include <dispatch/dispatch.h>

#include "stringConstants.h"

#ifdef bool
#undef bool
#endif

#ifdef true
#undef true
#undef false
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"

using namespace FSM;

int FSM::debugSuspends;              ///< debug suspend/resume/restart

int Machine::currentStateID() const
{
        return _currentState->stateID();
}


void Machine::executeOnEntry()
{
        _currentState->activity().performOnEntry(this);
}

void Machine::executeInternal()
{
        _currentState->activity().performInternal(this);
}

void Machine::executeOnExitForTransition(Transition *firingTransition)
{
        _previousState =_currentState;
        _currentState = firingTransition->target();     // target state
        //if (previousState()->activity())
        _previousState->activity().performOnExit(this); // onExit act
}

bool Machine::evaluateTransition(Transition *t)
{
        return t->expression()->evaluate(this) != 0;
}

bool Machine::executeOnce(bool *fired)
{
        assert(_currentState);                  // need a valid state

        /*
         * perform onEntry activities if this is a new state
         */
        if (_currentState != _previousState)    // entering a new state?
        {
                gettimeofday(&_state_time, NULL);
                _activities_count = 0;
                executeOnEntry();
                gettimeofday(&_actty_time, NULL);
        }

        /*
         * take a snapshot of the context (e.g. get Whiteboard content)
         */
        if (context()) context()->take_snapshot();

        /*
         * check all transitions to see if state change is required
         */
        Transition *firingTransition = NULL;
        for (TransitionVector::iterator it = _currentState->transitions().begin();
             it != _currentState->transitions().end(); it++)
        {
                Transition *t = *it;
                if (evaluateTransition(t))                      // does t fire?
                {
                        firingTransition = t;                   // yes, then
                        break;                                  // we are done
                }
        }
        _previousState = _currentState;

        /*
         * switch state and perform onExit activities if a transition fired
         */
        if (firingTransition)                   // new state required?
        {
                if (fired) *fired = true;
                executeOnExitForTransition(firingTransition);
                return true;
        }
        else if (fired) *fired = false;

        /*
         * no transition fired, so we perform internal activities
         */
        executeInternal();
        _activities_count++;

        /*
         * return and indicate whether the machine should keep going
         */
        return _currentState->transitions().size() != 0;
}

State *Machine::restart(State *initialState)
{
        using namespace std;

        State *oldState = _currentState;

        if (debugSuspends)
            cerr << "Restart " << id() << ": " << oldState->name() << " -> " << (initialState ? initialState->name() : states()[0]->name()) << (initialState ? "" : " (default)") << endl;

        initialise();
        if (initialState) _currentState = initialState;

        return oldState;
}

State *Machine::stateForID(int state_id) const
{
        for (StateVector::const_iterator it = const_states().begin();
             it != const_states().end(); it++)
        {
                State *s = *it;
                if (s->stateID() == state_id)
                        return s;
        }

        return NULL;
}

using namespace std;

string Machine::description() const
{
        stringstream ss;
        int i = 0;
        for (StateVector::const_iterator it = const_states().begin();
             it != const_states().end(); it++)
        {
                State *s = *it;
                ss << "State " << i++ << " (" << reinterpret_cast<long>(s) <<  "): " << s->description() << endl;
        }
        return ss.str();
}

string Machine::kripkeInSVMformat()
{
        stringstream ss;
        int i = 0;
        for (StateVector::const_iterator it = states().begin();
             it != states().end(); it++)
        {
                State *s = *it;
                ss << "State " << i++ << " (" << reinterpret_cast<long>(s) <<  "): " << s->description() << endl;
        }
        
        return ss.str();
}

const LocalKripkeFrezzePointVector &Machine::localKripkeStateNames(bool snapshotPerTransition)
{
        // caching
        if (_have_kripke_states)
                return _localKripkeStateNames;
        KripkeFreezePointOfMachine rigletCurrentStage;
        rigletCurrentStage.machine =this;
        rigletCurrentStage.transition_id = 0;
        
        int count=0;
        for (StateVector::const_iterator it = states().begin();
             it != states().end(); it++)
        {
                State *s = *it;
                rigletCurrentStage.stateID=s->stateID();
                rigletCurrentStage.ringletStage=Epcbefore;
                _localKripkeStateNames.push_back(rigletCurrentStage ) ;
                
                rigletCurrentStage.ringletStage=EpcAfterOnEntry;
                _localKripkeStateNames.push_back(rigletCurrentStage );
                                                                      
                /* we neeed information on transitions labels here */
                
                int transitionNumber=0;
   

                for (TransitionVector::iterator it_trans = s->transitions().begin();
                     it_trans != s->transitions().end(); it_trans++)
                {
                        rigletCurrentStage.transition_id=transitionNumber;
                        
                        if (snapshotPerTransition)	
                        {  rigletCurrentStage.ringletStage=EBeforeTransition;
                                _localKripkeStateNames.push_back(rigletCurrentStage );
                        }
                        
                        rigletCurrentStage.ringletStage=EtransitionFalse;
                        _localKripkeStateNames.push_back(rigletCurrentStage );
                        
                        rigletCurrentStage.ringletStage=EtransitionTrue;
                        _localKripkeStateNames.push_back(rigletCurrentStage );
                        

                       transitionNumber++;    
                } // each transiiton
                
                count++;
        }

        _have_kripke_states = true;
        return _localKripkeStateNames;
}

string Machine::initialStateName() const
{
        stringstream ss;
        
                ss << "M"<< id() << "$$" << pcBefore << (const_states()[0])->name();
 
        return ss.str();
}

bool Machine::evaluateTransitionWithIndex(int i)
{
        return evaluateTransition(_currentState->transitions()[i]);
}


void Machine::executeOnExitForTransitionWithIndex(int i)
{
        executeOnExitForTransition(_currentState->transitions()[i]);
}


size_t Machine::numberOfTransitionsInCurrentState()
{
        return _currentState->transitions().size();
}


/**
 * Convenience function to get the state time from the machine
 */
long long FSM::start_time_for_current_state(const Machine *machine)
{
        const timeval &state_time = machine->stateTime();
        long long usec = state_time.tv_sec * 1000000LL + state_time.tv_usec;

        return usec;
}


/**
 * Convenience function to get the current time of day
 */
long long FSM::current_time_in_microseconds(void)
{
        timeval current_time;
        gettimeofday(&current_time, NULL);
        long long usec = current_time.tv_sec * 1000000LL + current_time.tv_usec;

        return usec;
}

#pragma clang diagnostic pop
