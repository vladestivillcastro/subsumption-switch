/*
 *  clfsm_vector_factory.cc
 *  clfsm
 *
 *  Created by Rene Hexel on 19/09/12.
 *  Copyright (c) 2012 Rene Hexel. All rights reserved.
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
#ifndef __gufsm__clfsm_cc__
#define __gufsm__clfsm_cc__

//#include <cstdint>
#include <vector>
#include <string>
#include <iostream>

#ifndef USE_LIBCLANG_INTERNAL

//#include <clang-c/Index.h>

#else // use libclang internals

#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif
 
#include <clang/Driver/Arg.h>
#include <clang/Driver/ArgList.h>
#include <clang/Driver/Options.h>
#include <clang/Driver/DriverDiagnostic.h>
#include <clang/Driver/OptTable.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Frontend/FrontendDiagnostic.h>
#include <clang/Frontend/TextDiagnosticBuffer.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/FrontendTool/Utils.h>
#include <llvm/ADT/Statistic.h>
#include <llvm/Support/ErrorHandling.h>
#include <llvm/Support/ManagedStatic.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/Timer.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/LinkAllPasses.h>

#endif // USE_LIBCLANG_INTERNAL

namespace FSM
{
        class CcDelegate;

        class Cc
        {
                CcDelegate *_delegate;
#ifdef USE_LIBCLANG_INTERNAL
                llvm::OwningPtr<clang::CompilerInstance> clang;
                llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> diagIDs;
        public:
                /// constructor: takes a compiler instance and diagnostic IDs
                Cc(clang::CompilerInstance *ci, clang::DiagnosticIDs *dis): _delegate(NULL),
                        clang(ci), diagIDs(dis) {}
                /// default constructor: creates a new compiler instance
                Cc(): Cc(new clang::CompilerInstance(), new clang::DiagnosticIDs()) {}
#else
        public:
                Cc(): _delegate(NULL) {}
#endif

                /// default destructor
                virtual ~Cc() {}

                /// delegate getter
                CcDelegate *delegate() const { return _delegate; }

                /// delegate setter
                void setDelegate(CcDelegate *aDelegate) { _delegate = aDelegate; }

                /// set up: needs to be called only once in any program
                static void setup();

                /// tear down: needs to be called only once in any program
                static void teardown();

                /// compile using the given arguments
                virtual bool compile(std::vector<std::string> args, const char *argv0 = "clang++");

                /// standard error handler
                virtual void errorHandler(const std::string &message);
        };
}
#endif /* defined(__gufsm__clfsm_cc__) */
