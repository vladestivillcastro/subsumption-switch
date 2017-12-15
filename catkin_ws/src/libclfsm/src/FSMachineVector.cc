/*
 *  FSMachineVector.cc
 *  
 *  Created by René Hexel on 22/11/11.
 *  Copyright (c) 2011, 2013-2014 Rene Hexel.
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
#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdlib>
#include "FSMachineVector.h"
#include "FSMState.h"

#include "stringConstants.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"

using namespace FSM;
using namespace std;

static void default_idle_sleep(useconds_t t)
{
        protected_usleep(t);
}


StateMachineVector::StateMachineVector(Context *ctx, useconds_t timeout,
                                       idle_f default_idle_function):
        _context(ctx), _machines(), _idle_timeout(timeout), _accepting(false)
{
        stringstream ss;
        ss << "dispatch_queue_" << reinterpret_cast<long long>(this);

        _queue = dispatch_queue_create(ss.str().c_str(), NULL);

        if (!default_idle_function) default_idle_function = default_idle_sleep;
        _no_transition_fired = default_idle_function;
}


SuspensibleMachine *StateMachineVector::addMachine(SuspensibleMachine *m, int index, bool resume)
{
        int size = static_cast<int>(machines().size());
        int mid = index;
        if (mid < 0|| mid > size) mid = size;

        if (!m) m = new SuspensibleMachine(NULL, _context, 0, NULL, false);
        if (!m->id()) m->setID(mid);
        if (index < 0 || index >= size )
                _machines.push_back(m);
        else
        {
                SuspensibleMachine *old = _machines[index];
                _machines[index] = m;
                if (old)
                {
                        if (resume)
                        {
                                int sid = -1;
                                State *s = old->currentState();
                                if (s)
                                {
                                        sid = s->stateID();
                                        m->setCurrentState(m->stateForID(sid));
                                        s = old->previousState();
                                        if (s)
                                        {
                                                sid = s->stateID();
                                                m->setPreviousState(m->stateForID(sid));
                                        }
                                }
                        }
                        delete old;
                }
        }
        return m;
}


bool StateMachineVector::executeOnce(visitor_f should_execute_machine, void *context)
{
        int machine_no = 0;
        bool fired = false;

        setAccepting(true);

        for (MachineVector::iterator it = machines().begin();
             it != machines().end(); it++)
        {
                SuspensibleMachine *m = *it;

                if (should_execute_machine != NULL &&
                    !should_execute_machine(context, m, machine_no++))
                        continue;

                bool mfire = false;
                bool a = !m->executeOnce(&mfire);
                setAccepting(a && accepting());

                if (mfire) fired = true;
        }

        return fired;
}

void StateMachineVector::suspend()
{
        for (MachineVector::iterator it = machines().begin();
             it != machines().end(); it++)
        {
                SuspensibleMachine * m = *it;
                m->suspend();
        }
}

void StateMachineVector::resume()
{
        for (MachineVector::iterator it = machines().begin();
             it != machines().end(); it++)
        {
                SuspensibleMachine * m = *it;
                m->resume();
        }
}

void StateMachineVector::scheduleSuspend()
{
        for (MachineVector::iterator it = machines().begin();
             it != machines().end(); it++)
        {
                SuspensibleMachine * m = *it;
                m->scheduleSuspend();
        }
}

#ifndef __BLOCKS__
struct spawn_context
{
        SuspensibleMachine *m;
        bool &a, &f;
};

static void spawn_execute_once(void *p)
{
        spawn_context *c = static_cast<spawn_context *>(p);
        c->a = !c->m->executeOnce(&c->f);
}
#endif

bool StateMachineVector::executeOnceOnQueue(dispatch_queue_t queue)
{
        bool fired = false;

        if (!queue) queue = dispatch_get_main_queue();

        setAccepting(true);

        for (MachineVector::iterator it = machines().begin();
             it != machines().end(); it++)
        {
                SuspensibleMachine *m = *it;
                if (m->isSuspended() && !m->scheduledForResume() && !m->scheduledForRestart())
                        setAccepting(false);    // a suspended machine is never accepting
                else
                {
#ifdef __BLOCKS__
                        __block bool a = false, f = false;
                        dispatch_sync(queue, ^{ a = !m->executeOnce(&f); });
#else // no __BLOCKS__
                        bool a = false, f = false;
                        spawn_context c = { m, a, f };
                        dispatch_sync_f(queue, &c, spawn_execute_once);
#endif
                        setAccepting(a && accepting());

                        if (f) fired = true;
                }
        }        
        return fired;
}


void StateMachineVector::execute(visitor_f should_execute_machine, void *context)
{
        do
        {
                if (!executeOnce(should_execute_machine, context) &&
                    _no_transition_fired)
                        _no_transition_fired(_idle_timeout);
        }
        while (!accepting());
}


#ifndef __BLOCKS__
struct spawn_queue_param
{
        StateMachineVector *self;
        dispatch_queue_t queue;
};

 __attribute__((__noreturn__)) void StateMachineVector::do_spawn_once_on_queue(dispatch_queue_t queue)
{
        do
        {
                if (!executeOnceOnQueue(queue) && _no_transition_fired)
                        _no_transition_fired(_idle_timeout);
        }
        while (!accepting());

        exit(EXIT_SUCCESS);
}

 __attribute__((__noreturn__)) static void spawn_execute_once_on_queue(void *p)
{
        spawn_queue_param *par = static_cast<spawn_queue_param *>(p);
        par->self->do_spawn_once_on_queue(par->queue);
}
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#pragma clang diagnostic ignored "-Wweak-vtables"
#pragma clang diagnostic ignored "-Wpadded"


void StateMachineVector::scheduleExecuteOnQueue(dispatch_queue_t queue)
{
        if (!queue) queue = dispatch_get_main_queue();
#ifdef __BLOCKS__
        dispatch_async(_queue,
        ^{
                do
                {
                        if (!executeOnceOnQueue(queue) && _no_transition_fired)
                                _no_transition_fired(_idle_timeout);
                }
                while (!accepting());

                exit(EXIT_SUCCESS);
        });
#else // no __BLOCKS__
        spawn_queue_param p = { this, queue };
        dispatch_async_f(_queue, &p, spawn_execute_once_on_queue);
#endif
}

#pragma clang diagnostic pop

void StateMachineVector::initialise()
{
        for (MachineVector::iterator it = machines().begin();
             it != machines().end(); it++)
        {
                SuspensibleMachine *m = *it;
                m->initialise();
        }
}


void StateMachineVector::restart()
{
        for (MachineVector::iterator it = machines().begin();
             it != machines().end(); it++)
        {
                SuspensibleMachine *m = *it;
                m->restart();
        }
}


string StateMachineVector::description()
{
        stringstream ss;
        int i = 0;
        for (MachineVector::iterator it = machines().begin();
             it != machines().end(); it++)
        {
                SuspensibleMachine *m = *it;
                ss << "\nVector Machine " << i++ << ":\n" <<
                m->description() << endl;
        }
        return ss.str();
}

#pragma clang diagnostic pop
