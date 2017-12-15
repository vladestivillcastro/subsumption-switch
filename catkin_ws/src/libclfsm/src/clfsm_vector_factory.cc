/*
 *  clfsm_vector_factory.cc
 *  clfsm
 *
 *  Created by Rene Hexel on 5/09/12.
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
#include <dispatch/dispatch.h>
#include "FSMachineVector.h"
#include "FSMAsynchronousSuspensibleMachine.h"
#include "CLMachine.h"
#include "clfsm_factory.h"
#include "clfsm_vector_factory.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

using namespace std;
using namespace FSM;

static CLFSMVectorFactory *factory_singleton;

CLMachine *FSM::machine_at_index(unsigned index)
{
        if (!factory_singleton) return NULL;
        return factory_singleton->machine_at_index(index);
}


int FSM::index_of_machine_named(const char *name)
{
        if (!factory_singleton) return CLError;
        return factory_singleton->index_of_machine_named(name);
}


enum CLControlStatus FSM::control_machine_at_index(int index, enum CLControlStatus command)
{
        if (!factory_singleton) return CLError;
        return factory_singleton->control_machine_at_index(index, command);
}


int FSM::number_of_machines(void)
{
        if (!factory_singleton) return CLError;
        return int(factory_singleton->number_of_machines());
}

const char *FSM::name_of_machine_at_index(int index)
{
        if (!factory_singleton) return NULL;
        return factory_singleton->name_of_machine_at_index(index);
}


CLFSMVectorFactory::CLFSMVectorFactory(Context *context, bool del): _context(context), _clmachines(), _clfactories(), _delete(del)
{
        if (!factory_singleton) factory_singleton = this;
        _fsms = new StateMachineVector(context);
}


CLFSMVectorFactory::~CLFSMVectorFactory()
{
        if (factory_singleton == this) factory_singleton = NULL;
        if (_delete) delete _fsms;
}


SuspensibleMachine *CLFSMVectorFactory::addMachine(CLMachine *clm, int index, bool resume)
{
        int n = int(_clmachines.size());
        if (index < 0) index = n;
        else if (index > n) throw "invalid index";

        CLFSMFactory *factory = machine_factory(clm, index);

        if (index < n)
        {
                _clmachines[index] = clm;
                if (_clfactories[index]) delete _clfactories[index];
                _clfactories[index] = factory;
        }
        else
        {
                _clmachines.push_back(clm);
                _clfactories.push_back(factory);
        }

        SuspensibleMachine *m = factory->machine();
        _fsms->addMachine(m, index);

        return m;
}


CLFSMFactory *CLFSMVectorFactory::machine_factory(CLMachine *clm, int index)
{
        return new CLFSMFactory(_context, clm, index, _delete);
}


const char *CLFSMVectorFactory::name_of_machine_at_index(int i)
{
        int n = int(_clmachines.size());
        if (i < 0 || i >= n) return NULL;

        return _clmachines[i]->machineName();
}


int CLFSMVectorFactory::index_of_machine_named(const char *machine_name)
{
        if (!machine_name) return CLError;

        size_t n = _clmachines.size();
        if (!machine_name) return int(n)-1;
        for (size_t i = 0; i < n; i++)
        {
                if (strcmp(_clmachines[i]->machineName(), machine_name) == 0)
                        return int(i);
        }

        string machine_name_with_extension(machine_name);
        machine_name_with_extension += ".machine";
        const char *name = machine_name_with_extension.c_str();
        for (size_t i = 0; i < n; i++)
        {
                if (strcmp(_clmachines[i]->machineName(), name) == 0)
                        return int(i);
        }

        return CLError;
}

#define MSTATUS(m)      ((m)->isSuspended() ? CLSuspend : CLStatus)

enum CLControlStatus CLFSMVectorFactory::control_machine_at_index(int i, enum CLControlStatus command)
{
        int n = int(_clmachines.size());
        if (i < 0 || i >= n) return CLError;

        AsynchronousSuspensibleMachine *m = static_cast<AsynchronousSuspensibleMachine *>(_fsms->machines()[i]);
        enum CLControlStatus status = MSTATUS(m);
        switch (command)
        {
                case CLStatus:
                        break;
                        
                case CLSuspend:
                        m->scheduleSuspend();
                        break;
                        
                case CLResume:
                        m->scheduleResume();
                        break;
                        
                case CLRestart:
                        m->scheduleRestart();
                        break;

                case CLError:
                        status = CLError;
                        break;
        }

        return status;
}

#pragma clang diagnostic pop
