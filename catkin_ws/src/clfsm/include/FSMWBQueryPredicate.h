/*
 *  FSMWBQueryPredicate.h
 *  
 *  Created by René Hexel on 22/10/11.
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
#ifndef gufsm_FSMWBQueryPredicate_h
#define gufsm_FSMWBQueryPredicate_h

#include <pthread.h>
#include "FSMWBPredicate.h"
#include "FSMWBContext.h"

#ifdef bool
#undef bool
#endif

#ifdef true
#undef true
#undef false
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"

namespace guWhiteboard
{
        class Whiteboard;
}

namespace FSM
{
        class Machine;

        /**
         * Complex whiteboard predicate that asks an expert first and waits
         * for a response
         */
        class WBQueryPredicate: public WBPredicate
        {
                pthread_mutex_t _lock;          ///< condition lock
                pthread_cond_t _receivedProof;  ///< proof received condition
                bool _waiting;                  ///< waiting for a callback?

                void init(guWhiteboard::Whiteboard *wb);
                void callback(std::string dataName, WBMsg *msg);
        public:
                /**
                 * Whiteboard constructor
                 * @param p whiteboard message name to get
                 * @param neg is this a negation?
                 * @param wb the whiteboard to use (null to use machine context)
                 */
                WBQueryPredicate(const std::string &p, bool neg, guWhiteboard::Whiteboard *wb = NULL);

                /**
                 * Whiteboard context constructor
                 * @param p whiteboard message name to get
                 * @param neg is this a negation?
                 * @param wc the whiteboard context to use (may not be null)
                 */
                WBQueryPredicate(const std::string &p, bool neg, WBContext *wc);

                virtual ~WBQueryPredicate() {}  /// destructor

                /** whiteboard setter (subscribes to message responses) */
                void setWhiteboard(guWhiteboard::Whiteboard *wb);

                /** return the value, negated if necessary */
                virtual int evaluate(Machine *m = NULL);
        };
}

#pragma clang diagnostic pop

#endif
