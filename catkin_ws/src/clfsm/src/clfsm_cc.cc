/*
 *  clfsm_vector_factory.cc
 *  clfsm
 *
 *  Created by Rene Hexel on 19/09/12.
 *  Copyright (c) 2012, 2013-2014 Rene Hexel. All rights reserved.
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
#include <cstdlib>
#include <cstdio>
#include <cerrno>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-macros"
#undef __block
#define __block _xblock
#include <unistd.h>
#undef __block
#define __block __attribute__((__blocks__(byref)))
#pragma clang diagnostic pop

#include <sys/wait.h>
#include <vector>
#include <gu_util.h>
#include "clfsm_cc_delegate.h"
#include "clfsm_cc.h"

#pragma GCC diagnostic push
#pragma clang diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma clang diagnostic ignored "-Wunused-function"

#ifdef USE_LIBCLANG_INTERNAL
using namespace llvm;
using namespace clang;
#endif
using namespace std;
using namespace FSM;

void Cc::setup()
{
#ifdef USE_LIBCLANG_INTERNAL
        static dispatch_once_t once;

        dispatch_once(&once,
        ^{
                InitializeAllTargets();
                InitializeAllTargetMCs();
                InitializeAllAsmPrinters();
                InitializeAllAsmParsers();
        });
#endif
}


void Cc::teardown()
{
#ifdef USE_LIBCLANG_INTERNAL
        static dispatch_once_t once;
        
        dispatch_once(&once,
        ^{
                llvm_shutdown();
        });
#endif
}


#ifdef USE_LIBCLANG_INTERNAL
static void error_handler(void *cc, const std::string &message)
{
        Cc *self = static_cast<Cc *>(cc);
        self->errorHandler(message);
}
#endif

void Cc::errorHandler(const std::string &message)
{
        if (delegate())
        {
                delegate()->handleError(this, message);
                return;
        }
#ifdef USE_LIBCLANG_INTERNAL
        clang->getDiagnostics().Report(diag::err_fe_error_backend) << message;
#else
        cerr << message << endl;
#endif
}


bool Cc::compile(vector<string> args, const char *argv0)
{
#ifndef USE_LIBCLANG_INTERNAL
        bool success = true;
        pid_t pid = fork();

        switch (pid)
        {
                case -1:
                        cerr << "Cannot fork " << argv0 << ": " << strerror(errno) << endl;
                        return false;

                case 0:
                {
                        ssize_t n = args.size();
                        vector<char *> exec_args;
                        exec_args.reserve(n+2);
                        exec_args[0] = const_cast<char *>(argv0);
                        DBG(cout << argv0 << " ");
                        for (int i = 0; i < n; i++)
                        {
                                exec_args[i+1] = const_cast<char *>(args[i].c_str());
                                DBG(cout << args[i] << " ");
                        }
                        DBG(cout << endl);
                        exec_args[n+1] = NULL;
                        execvp(argv0, &exec_args[0]);
                        cerr << "Cannot exec " << argv0 << endl;
                        exit(EXIT_FAILURE);
                }

                default:
                        break;
        }

        int status = 0;
        while (waitpid(pid, &status, 0) != pid) if (errno != EAGAIN && errno != EINTR)
        {
                if (errno == ENOENT)    // child exited before we got there
                        break;
                cerr << "Error executing " << argv0 << ": " << strerror(errno) << " (" << errno << ")" << endl;
                return false;
        }
        if (WIFSIGNALED(status))
        {
                cerr << argv0 << " exited due to signal " << WTERMSIG(status) << endl;
                return false;
        }
        if (WIFSTOPPED(status))
        {
                cerr << argv0 << " stopped due to signal " << WSTOPSIG(status) << endl;
                return false;
        }
        if (WCOREDUMP(status))
        {
                cerr << "Error " << argv0 << " dumped core" << endl;
                return false;
        }
        if (WEXITSTATUS(status) != EXIT_SUCCESS)
        {
                cerr << argv0 << " exited with return code " << WEXITSTATUS(status) << endl;
                return false;
        }
#else
        TextDiagnosticBuffer *diagsBuffer = new TextDiagnosticBuffer;
        if (!diagsBuffer)
                return false;
        //
        // buffer diagnostics from argument parsing so that we can output them
        // using a well formed diagnostic object
        //
        DiagnosticsEngine diags(diagIDs, diagsBuffer);
        bool success = CompilerInvocation::CreateFromArgs(clang->getInvocation(),
                                                          argBegin, argEnd, diags);
        if (!success)
                goto fin;
        //
        // infer the builtin include path if unspecified
        //
        if (clang->getHeaderSearchOpts().UseBuiltinIncludes &&
            clang->getHeaderSearchOpts().ResourceDir.empty())
                clang->getHeaderSearchOpts().ResourceDir = CompilerInvocation::GetResourcesPath(argv0, mainAddr);
        //
        // Create the actual diagnostics engine.
        //
        clang->createDiagnostics(argEnd - argBegin, const_cast<char **>(argBegin));
        if (!clang->hasDiagnostics())
        {
                success = false;
                goto fin;
        }

        install_fatal_error_handler(error_handler, static_cast<void *>(this));
        diagsBuffer->FlushDiagnostics(clang->getDiagnostics());

        success = ExecuteCompilerInvocation(clang.get());

        TimerGroup::printAll(errs());
        remove_fatal_error_handler();

        //
        // When running with -disable-free, don't do any destruction or shutdown.
        //
        if (clang->getFrontendOpts().DisableFree)
        {
                if (AreStatisticsEnabled() || clang->getFrontendOpts().ShowStats)
                        PrintStatistics();
                clang.take();
        }
fin:
        if (!db) delete diagsBuffer;
#endif // USE_LIBCLANG_INTERNAL

        return success;
}

#pragma clang diagnostic pop
#pragma GCC diagnostic pop
