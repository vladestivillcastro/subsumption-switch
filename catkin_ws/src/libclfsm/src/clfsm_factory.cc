/*
 *  clfsm_factory.cc
 *  clfsm
 *
 *  Created by Rene Hexel on 5/08/12.
 *  Copyright (c) 2012, 2013 Rene Hexel. All rights reserved.
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
#include "FSMAsynchronousSuspensibleMachine.h"
#include "FSMTransition.h"
#include "FSMState.h"
#include "CLMachine.h"
#include "CLState.h"
#include "CLTransition.h"
#include "CLTransitionExpression.h"
#include "CLActionAction.h"
#include "clfsm_factory.h"

#ifdef bool
#undef bool
#endif

#ifdef true
#undef true
#undef false
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wvla"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#pragma clang diagnostic ignored "-Wvla-extension"
#pragma clang diagnostic ignored "-Wunused-parameter"

using namespace std;
using namespace FSM;

CLFSMFactory::CLFSMFactory(Context *context, CLMachine *clm, int mid, bool del): Factory(NULL, del), _clm(clm)
{
        State *initialState = NULL;
        State *suspendState = NULL;
        CLState * const *cl_states = clm->states();
        const CLState * cl_initial = clm->initialState();
        const CLState * cl_suspend = clm->suspendState();
        int n = clm->numberOfStates();
        State *states[n];

        /*
         * create a state for each CL state
         */
        for (int i = 0; i < n; i++)
        {
                CLState *clstate = cl_states[i];
                State *state = createState(clstate, i);
                states[i] = state;
                if (clstate == cl_initial) initialState = state;
                else if (clstate == cl_suspend) suspendState = state;
        }

        /*
         * create a machine for the CL machine
         */
        createMachine(clm, context, initialState, mid, clm->machineName());
        SuspensibleMachine *fsm = machine();
        for (int i = 0; i < n; i++)
        {
                CLState *clstate = cl_states[i];
                State *state = states[i];
                fsm->addState(state);
                createActions(clm, clstate, state);
                createTransitions(clm, clstate, state, states);
        }

        if (suspendState)
                fsm->setSuspendState(suspendState);
        else
                determineSuspendState();
}


void CLFSMFactory::createMachine(CLMachine *clm, Context *context, State *initialState, int mid, const char *name)
{
        AsynchronousSuspensibleMachine *fsm = new AsynchronousSuspensibleMachine(initialState, context, mid);
        setMachine(fsm);
        clm->setMachineContext(fsm);
}


State *CLFSMFactory::createState(CLState *clstate, int state_number)
{
        return new State(state_number, clstate->name());
}


void CLFSMFactory::createActions(CLMachine *clm, CLState *clstate, State *state)
{
        CLActionAction *onEntry  = new CLActionAction(clm, clstate, &clstate->onEntryAction());
        CLActionAction *onExit   = new CLActionAction(clm, clstate, &clstate->onExitAction());
        CLActionAction *internal = new CLActionAction(clm, clstate, &clstate->internalAction());

        state->activity().addOnEntryAction(onEntry);
        state->activity().addOnExitAction(onExit);
        state->activity().addInternalAction(internal);
}


void CLFSMFactory::createTransitions(CLMachine *clm, CLState *clstate, State *state, State **states)
{
        CLTransition * const *cl_transitions = clstate->transitions();
        int n = clstate->numberOfTransitions();
        state->transitions().reserve(n);
        for (int i = 0; i < n; i++)
        {
                CLTransition *cl_transition = cl_transitions[i];
                State *dest = states[cl_transition->destinationState()];
                CLTransitionExpression *expression = new CLTransitionExpression(clm, clstate, cl_transition);
                Transition *transition = new Transition(state, dest, expression);
                state->addTransition(transition);
        }
}


CLFSMFactory::~CLFSMFactory()
{
        int n = _clm->numberOfStates();

        /*
         * tear down each state
         */
        for (int i = 0; i < n; i++)
        {
                CLState *clstate = _clm->state(i);
                State *state = machine()->states()[i];
                CLTransition * const *cl_transitions = clstate->transitions();
                int m = clstate->numberOfTransitions();
                for (int j = 0; j < m; j++)
                {
                        CLTransition *cltransition = cl_transitions[j];
                        Transition *transition = state->transitions()[j];
                        Expression *expression = transition->expression();

                        delete expression;
                        delete transition;
                        delete cltransition;
                }

                for (ActionVector::iterator j = state->activity().onEntryActions().begin(); j !=  state->activity().onEntryActions().end(); j++)
                        delete *j;

                for (ActionVector::iterator j = state->activity().onExitActions().begin(); j !=  state->activity().onExitActions().end(); j++)
                        delete *j;

                for (ActionVector::iterator j = state->activity().internalActions().begin(); j !=  state->activity().internalActions().end(); j++)
                        delete *j;

                delete state;
                delete clstate;
        }

        delete _clm;
}

#pragma clang diagnostic pop
