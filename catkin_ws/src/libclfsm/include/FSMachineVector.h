/*
 *  FSMachineVector.h
 *  
 *  Created by René Hexel on 22/11/11.
 *  Copyright (c) 2011, 2013 Rene Hexel.
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
#ifndef gufsm_FSMachineVector_h
#define gufsm_FSMachineVector_h

#include <dispatch/dispatch.h>
#undef __block
#define __block _xblock
#include <unistd.h>
#undef __block
#define __block __attribute__((__blocks__(byref)))
#include <vector>
#include <string>
#include <list>
#include <iostream>
#include <utility>
#include "gu_util.h"
#include "FSMSuspensibleMachine.h"

#ifdef bool
#undef bool
#endif

#ifdef true
#undef true
#undef false
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#pragma clang diagnostic ignored "-Wunused-parameter"

namespace FSM
{
        typedef std::vector<SuspensibleMachine *> MachineVector;
        typedef MachineVector::iterator MachineIterator;
        typedef void (*idle_f)(useconds_t timeout);
        typedef bool (*visitor_f)(void *context, SuspensibleMachine *machine, int machine_number);

        typedef std::vector<KripkeFreezePointOfMachine> KripkeFreezePointVector; // single freeze point across all machines

        struct KripkeState
        {
                unsigned long long variable_combination;
                KripkeFreezePointVector *freeze_point;
                int whose_turn;         // current machine's turn (id)

                KripkeState(unsigned long long v, KripkeFreezePointVector *f, int w=0): variable_combination(v), freeze_point(f), whose_turn(w) {}
                KripkeState(const KripkeState &other): variable_combination(other.variable_combination), freeze_point(other.freeze_point), whose_turn(other.whose_turn)
                {
                       DBG( std::cout << "Freeze point " << (unsigned long) freeze_point << std::endl);
                }
                KripkeState &operator=(const KripkeState &other)
                {
                        variable_combination = other.variable_combination;
                        whose_turn = other.whose_turn;
                        freeze_point = other.freeze_point;
                       DBG( std::cout << "Freeze point " << (unsigned long) freeze_point << std::endl);
                        return *this;
                }
                bool operator==(const struct KripkeState &other) const
                {       
                        if (variable_combination != other.variable_combination ||
                        whose_turn != other.whose_turn)
                                return false;
                        KripkeFreezePointVector::iterator it = (*other.freeze_point).begin();
                        KripkeFreezePointVector::iterator it2 = freeze_point->begin();
                        while (it2 != freeze_point->end())
                        {
                                if (it == (*other.freeze_point).end())
                                        return false;
                                if (*it2++ != *it++)
                                        return false;
                        }
                        return true;
                }
        };


        /**
         * State machine vector class
         */
        class StateMachineVector
        {
        protected:
                Context         *_context;              ///< global context
                MachineVector   _machines;              ///< vector of suspensible FSMs
                idle_f          _no_transition_fired;   ///< idle function
                dispatch_queue_t _queue;                ///< dispatch run queue
                useconds_t      _idle_timeout;          ///< idle timeout in usec
                bool            _accepting;             ///< all machines are in an accepting state

                /// subclass responsibility
                std::string generate_from(KripkeState &, std::list<KripkeState> &, size_t n, std::string **names) { return ""; }
                /// subclass responsibility
                std::string kripkeToString(KripkeState &s, size_t n, std::string **names, bool derived=false) { return ""; }
        public:
                /** constructor */
                StateMachineVector(Context *ctx = NULL, useconds_t timeout = 10000L, idle_f default_idle_function = NULL);

                /** destructor */
                virtual ~StateMachineVector() {}

                /** machines getter method */
                MachineVector &machines() { return _machines; }
                
                /** machines setter method */
                void setMachines(const MachineVector &mv) { _machines = mv; }
                
                /**
                 * add a machine at a given index (-1 = end)
                 * @param m             state machine to add (NULL to create a new one)
                 * @param index         index if replacing an existing state machine (-1 to add at end)
                 * @param resume        true if new state machine should resume from state where old machine left off
                 */
                SuspensibleMachine *addMachine(SuspensibleMachine *m = NULL, int index=-1, bool resume=false);

                /** context getter */
                Context *context() { return _context; }
                
                /** context setter */
                void setContext(Context *ctx = NULL) { _context = ctx; }
                
                /** accepting state getter */
                bool accepting() { return _accepting; }
                
                /** accepting state setter */
                void setAccepting(bool accept = true) { _accepting = accept; }
                
                /** put all state machines into their initial state */
                virtual void initialise();

                /**
                 * execute one iteration of the current state of each machine
                 * @return true if any transition fired on any machine
                 */
                virtual bool executeOnce() { return executeOnce(NULL); }

                /**
                 * execute one iteration of the current state of each machine
                 * with a given visitor for each machine
                 * @param should_execute_machine visitor that returns whether machine should be executed in this round
                 * @return true if any transition fired on any machine
                 */
                virtual bool executeOnce(visitor_f should_execute_machine, void *context = NULL);

                /**
                 * synchronously execute once on a specific dispatch queue
                 */
                virtual bool executeOnceOnQueue(dispatch_queue_t queue = NULL);
                
                /**
                 * execute until accepting state is encountered
                 */
                virtual void execute() { execute(NULL); }

                /**
                 * execute until accepting state is encountered
                 */
                virtual void execute(visitor_f should_execute_machine, void *context = NULL);

                /**
                 * asynchronously schedule execute on a specific dispatch queue until
                 * accepting state is encountered
                 */
                virtual void scheduleExecuteOnQueue(dispatch_queue_t queue = NULL);
 
                /**
                 * execute dedicated function if no transition fired --
                 * sleeps for _idle_timeout by default
                 */
                virtual void noTransitionFired(void) { if (_no_transition_fired) _no_transition_fired(_idle_timeout); }

                /**
                 * subclass responsibility:
                 * print the Kripke structure in svm format 
                 */
                virtual std::string kripkeInSVMformat() { return ""; }

                /**
                 * restart all state machines from their initial state
                 */
                virtual void restart();
                
                /**
                 * call suspend() on all state machines.
                 */
                virtual void suspend();
                
                /**
                 * call resume() on all state machines.
                 */
                virtual void resume();

                /**
                 * schedule suspend() on all state machines.
                 */
                virtual void scheduleSuspend();

                /**
                 * printable state machine vector description
                 */
                virtual std::string description();
                
                /** 
                 * subclass responsibility: serialise a Kripke Gobal vector in smv format
                 */
                std::string descriptionSMVformat(KripkeFreezePointVector &) { return ""; }
#ifndef __BLOCKS__
                /** not really public */
                void do_spawn_once_on_queue(dispatch_queue_t queue);
#endif
        };
}

#pragma clang diagnostic pop

#endif
