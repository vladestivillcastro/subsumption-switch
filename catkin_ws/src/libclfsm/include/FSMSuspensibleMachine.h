/*
 *  FSMSuspensibleMachine.h
 *  
 *  Created by René Hexel on 24/09/11.
 *  Copyright (c) 2011-2014 Rene Hexel.
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
#ifndef gufsm_FSMSuspensibleMachine_h
#define gufsm_FSMSuspensibleMachine_h

#include "FSMachine.h"

#ifdef bool
#undef bool
#endif

#ifdef true
#undef true
#undef false
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wunused-parameter"

namespace FSM
{
        class State;

        class SuspensibleMachine: public Machine
        {
                State *_suspendState;           ///< the suspend state
                State *_resumeState;            ///< state to resume to
                bool _deleteSuspendState;       ///< should delete in destructor?
        public:
                /** constructor */
                SuspensibleMachine(State *initialState = NULL, 
                                   Context *ctx = NULL, int mid=0,
                                   State *s = NULL, bool del = false):
                                Machine(initialState, ctx, mid),
                                _suspendState(s),
                                _resumeState(NULL),
                                _deleteSuspendState(del) {}
                /** destructor */
                virtual ~SuspensibleMachine();

                /** suspend state getter method */
                State *suspendState() const { return _suspendState; }

                /** suspend state setter */
                void setSuspendState(State *s, bool del = false);

                /** tell whether this machine is suspended */
                bool isSuspended() const { return _suspendState && currentState() == _suspendState; }

                /** suspend this state machine */
                virtual void suspend();

                /** resume this state machine where it left off */
                virtual void resume();

                /** is this machine scheduled for resumption? */
                virtual bool scheduledForResume() const { return false; }

                /** is this machine scheduled for restart? */
                virtual bool scheduledForRestart() const { return false; }

                /** schedule suspend (subclass responsibility */
                virtual void scheduleSuspend(bool s=true) { if (s) suspend(); }
        };
}

#pragma clang diagnostic pop

#endif
