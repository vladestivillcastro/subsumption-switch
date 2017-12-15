/*
 *  FSMSuspensibleMachine.cc
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
#include "FSMSuspensibleMachine.h"
#include "FSMState.h"

using namespace FSM;

SuspensibleMachine::~SuspensibleMachine()
{
        if (_deleteSuspendState && _suspendState)
                delete _suspendState;
}


void SuspensibleMachine::setSuspendState(State *s, bool del)
{
        if (_deleteSuspendState && _suspendState)
                delete _suspendState;

        _suspendState = s;        
        _deleteSuspendState = del;
}


void SuspensibleMachine::suspend()
{
        using namespace std;

        if (!_suspendState)
        {
                _suspendState = new State(-1, "Suspended");
                if (!_suspendState)
                        throw "Unable to create dynamic suspend state";
                _deleteSuspendState = true;
        }

        if (currentState() != _suspendState)
        {
                _resumeState = currentState();
                setPreviousState(_resumeState);
                if (debugSuspends)
                    cerr << "Suspend " << id() << ": " << previousState()->name() << " -> " << currentState()->name() << endl;
        }
        else if (debugSuspends > 1)
            cerr << "Suspend " << id() << ": " << previousState()->name() << " -> " << currentState()->name() << " (re-suspend)" << endl;

        setCurrentState(_suspendState);
}


void SuspensibleMachine::resume()
{
        using namespace std;

        State *curr = currentState();

        /*
         * only do anything if suspended
         */
        if (curr != _suspendState)
        {
                if (debugSuspends > 1)
                        cerr << "Resume " << id() << ": " << currentState()->name() << " -> " << currentState()->name() << " (re-resume)" << endl;
                return;
        }

        State *prev = _resumeState;
        if (!prev || prev == _suspendState) prev = states()[0];

        setPreviousState(curr);
        setCurrentState(prev);

        if (debugSuspends)
                cerr << "Resume " << id() << ": " << curr->name() << " -> " << prev->name() << (prev == states()[0] ? " (initial)" : "") << endl;
}

