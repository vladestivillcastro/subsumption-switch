/*
 *  clfsm_vector_factory.h
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
#ifndef ____clfsm_vector_factory__
#define ____clfsm_vector_factory__

#include <vector>

#ifdef bool
#undef bool
#endif

#ifdef true
#undef true
#undef false
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"


namespace FSM
{
        class CLMachine;
        class CLFSMFactory;
        class SuspensibleMachine;
        class StateMachineVector;
        class Context;

        enum CLControlStatus
        {
                CLError = -1,   ///< error indicator
                CLStatus,       ///< check status only (running status when used as a return code)
                CLSuspend,      ///< suspend the corresponding state machines
                CLResume,       ///< resume the corresponding state machines
                CLRestart       ///< restart the corresponding state machine
        };

        /// return the total number of machines in the factory singleton
        int number_of_machines(void);

        /// return a pointer to the machine at a given index in the vector
        CLMachine *machine_at_index(unsigned index);

        /// return the name of the machine in the factory singleton at the given index (NULL if the index is invalid)
        const char *name_of_machine_at_index(int index = 0);

        /// return the index of a machine with a given name in the factory singleton
        int index_of_machine_named(const char *machine_name = NULL);

        /// control the machine at the given index in the factory singleton
        enum CLControlStatus control_machine_at_index(int index = 0, enum CLControlStatus command = CLStatus);

        /**
         * Finite State machine Vector Factory
         */
        class CLFSMVectorFactory
        {
        protected:
                StateMachineVector      *_fsms;                 ///< created FSMs
                Context                 *_context;              ///< factory context
                std::vector<CLMachine *> _clmachines;           ///< CL machines in vector
                std::vector<CLFSMFactory *> _clfactories;       ///< factories
                bool                     _delete;               ///< delete machines?
        public:
                /// default constructor
                CLFSMVectorFactory(Context *context, bool del = true);
                virtual ~CLFSMVectorFactory();           /// destructor

                /// state machine vector getter
                StateMachineVector *fsms() const { return _fsms; }

                /// delete mode getter
                bool deleteOnDealloc() const { return _delete; }

                /// delete mode setter
                void setDeleteOnDealloc(bool del = true) { _delete = del; }

                /** add a machine to the vector */
                virtual SuspensibleMachine *addMachine(CLMachine *clm, int index=-1, bool resume=false);

                /// return the machine factory to use (override in subclasses)
                virtual CLFSMFactory *machine_factory(CLMachine *clm, int index);

                /// return the number of machines in the vector
                size_t number_of_machines() const { return _clmachines.size(); }

                /// return the name of the machine at the given index
                const char *name_of_machine_at_index(int i = 0);

                /**
                 * return the index of the machine with the given name (-1 if not found)
                 * @param machine_name  name of the machine to search for (NULL = last machine)
                 * @return index of that machine or -1 if not found
                 */
                int index_of_machine_named(const char *machine_name = NULL);

                /**
                 * control the machine at a given index and return the status for that machine
                 * @param index         number of the machine in question [0..n)
                 * @param command       command to execute (CLSuspend, CLResume, CLRestart, CLStatus)
                 * @return status of the machine or CLError in case of an error
                 */
                enum CLControlStatus control_machine_at_index(int index = 0, enum CLControlStatus command = CLStatus);

                /**
                 * return the machine at a given index (const version)
                 */
                const CLMachine *machine_at_index(size_t index = 0) const
                {
                        return (index < _clmachines.size()) ? _clmachines[index] : NULL;
                }

                /**
                 * return the machine at a given index
                 */
                CLMachine *machine_at_index(size_t index = 0)
                {
                        return (index < _clmachines.size()) ? _clmachines[index] : NULL;
                }
        };
}

#pragma clang diagnostic pop

#endif /* defined(____clfsm_factory__) */
