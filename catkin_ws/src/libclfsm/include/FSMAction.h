/*
 *  Action.h
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
#ifndef gufsm_FSMAction_h
#define gufsm_FSMAction_h

#include <cstdarg>
#include <string>
#include <sstream>
#include <iostream>

#include "FSMExpression.h"

#ifdef bool
#undef bool
#endif

#ifdef true
#undef true
#undef false
#endif

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#pragma clang diagnostic ignored "-Wweak-vtables"
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#pragma clang diagnostic ignored "-Wunused-parameter"

namespace FSM
{
        class Machine;

        enum ActionStage { STAGE_ON_ENTRY, STAGE_ON_EXIT, STAGE_INTERNAL,
                NUM_ACTION_STAGES };
        /**
         * Abstract class for FSM actions
         */
        class Action: public Expression
        {
        public:
                /** virtual destructor */
                virtual ~Action() {}

                /** perform the given action with a vector of parameters */
                virtual void performv(Machine *, ActionStage, int, va_list) = 0;
                
                /** perform the given action */
                virtual void perform(Machine *m, ActionStage stage = NUM_ACTION_STAGES, int ac = 0, ...)
                {
                        va_list ap;
                        va_start(ap, ac);
                        performv(m, stage, ac, ap);
                        va_end(ap);
                }

                /** action description */
                virtual std::string description();

                /** evaluation by default just performs, then returns 1 */
                virtual int evaluate(Machine *m = NULL) { perform(m); return true; }
        };
        
        
        /**
         * Abstract template class for FSMs actions with simple content
         */
        template <typename T> class ContentAction: public Action
        {
        protected:
                T _content;             /// content relevant for this action
        public:
                /** default constructor */
                ContentAction() {}

                /** designated constructor */
                ContentAction(const T &content): _content(content) {}
                
                /** getter method */
                virtual const T &content() const { return _content; }
                
                /** setter method */
                virtual void setContent(const T &c) { _content = c; }

                /** action description */
                virtual std::string description()
                {
                        std::stringstream ss;
#ifdef COMPLEX_CONTENT_ACTION
                        ss << "Content Action " << &content() << std::endl;
#else
                        ss << "Content Action " << content() << std::endl;
#endif
                        return ss.str();
                }
        };
        
        
        /**
         * Printing action
         */
        template <typename T> class PrintingAction: public ContentAction<T>
        {
        public:
                /** printing action default constructor */
                PrintingAction(T val): ContentAction<T>(val) {}

                /** print the content of this action */
                virtual void performv(Machine *, ActionStage, int, va_list)
                {
                        std::cout << this->_content << std::endl;
                }
                
                /** action description */
                virtual std::string description()
                {
                        std::stringstream ss;
                        ss << "Printing Action " << ContentAction<T>::content()
                           << std::endl;
                        return ss.str();
                }
        };
        
        /**
         * Printing action for strings
         */
        class PrintStringAction: public PrintingAction<std::string>
        {
        public:
                PrintStringAction(std::string s = ""): PrintingAction<std::string>(s) {}
                
                /** setting any parameter sets the context */
                virtual void add_parameter(int index, long long value)
                {
                        setContent(reinterpret_cast<const char *>(value));
                }
        };

        /**
         * Printing action for strings (no newline)
         */
        class PrintStringNoNLAction: public PrintStringAction
        {
        public:
                /** print the content of this action */
                virtual void performv(Machine *, ActionStage, int, va_list)
                {
                        std::cout << _content;
                }
                
        };

        /**
         * Printing action for ints
         */
        class PrintIntAction: public PrintingAction<int>
        {
        public:
                /** default constructor */
                PrintIntAction(int val = 0): PrintingAction<int>(val) {}

                /** setting any parameter sets the context */
                virtual void add_parameter(int index, long long value)
                {
                        setContent(int(value));
                }
        };

        /**
         * Delay action: specify sleep time in milliseconds
         */
        class SleepAction: public ContentAction<int>
        {
        public:
                /** sleep action default constructor */
                SleepAction(int val = 1000): ContentAction<int>(val) {}
                
                /** sleep for the given amount of milliseconds */
                virtual void performv(Machine *, ActionStage, int, va_list);
                
                /** action description */
                virtual std::string description();
        };
}

#pragma clang diagnostic pop

#endif
