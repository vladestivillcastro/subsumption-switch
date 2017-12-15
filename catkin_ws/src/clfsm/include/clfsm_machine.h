/*
 *  clfsm_machine.h
 *  clfsm
 *
 *  Created by Rene Hexel on 11/10/12.
 *  Copyright (c) 2012, 2013, 2014 Rene Hexel. All rights reserved.
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
#ifndef __gufsm__clfsm_machine__
#define __gufsm__clfsm_machine__

#include <string>
#include <vector>
#include "clfsm_cc_delegate.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#pragma clang diagnostic ignored "-Wweak-vtables"
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"

namespace FSM
{
        class CLMachine;

        typedef CLMachine* (*create_machine_f)(int, const char *);

        class MachineWrapper
        {
                std::string _fullPath;          ///< full name, including path
                std::string _name;              ///< name w/o path and extension
                create_machine_f _factory;      ///< machine factory
                void *_shared_object;           ///< object as returned by dlopen()
                class Cc *_compiler;            ///< C++ compiler wrapper
                const std::vector<std::string> *_compiler_args; ///< compiler command line arguments
                const std::vector<std::string> *_linker_args;   ///< linker command line arguments
                bool _delete_compiler;          ///< does the compiler need deletion?
        public:
                MachineWrapper(std::string path);
                virtual ~MachineWrapper();

                /// change the name of a machine (e.g. for uniquing)
                void setName(std::string name) { _name = name; }

                /// return the name of the machine (without path and extension)
                const char *name() const { return _name.c_str(); }

                /// return the full file system path of the machine
                const char *path() const {return _fullPath.c_str(); }

                /// return the full file system path of the binary directory
                std::string binaryDirectory() const;

                /// return the full file system path of the include path file
                std::string includePathFile() const;

                /// set the compiler
                virtual void setCompiler(Cc *compiler = NULL, bool del = false);

                /// get the compiler
                Cc *compiler() const { return _compiler; }

                /// set the compiler arguments
                virtual void setCompilerArgs(const std::vector<std::string> &args) { _compiler_args = &args; }

                /// set the linker arguments
                virtual void setLinkerArgs(const std::vector<std::string> &args) { _linker_args = &args; }

                /// return the states of the given machine
                std::vector<std::string> states() const;

                /// compile the given machine
                virtual bool compile(const std::vector<std::string> &args, const std::vector<std::string> &linker_args);

                /// compile the given machine with same args for compiler and linker
                bool compile(const std::vector<std::string> &args) { return compile(args, args); }

                /// instantiate a machine
                CLMachine *instantiate(int id, const char *machine_name);

                /// return the default compiler args
                static const std::vector<std::string> &default_compiler_args();

                /// return the default linker args
                static const std::vector<std::string> &default_linker_args();

                /// add include directories from the given machine (if any) to the given compiler arguments
                virtual void add_machine_includes(std::vector<std::string> &compiler_args);

                /// return a string with $FOO or ${FOO} replaced with environment variable
                static std::string stringByExpandingEnvironmentVariablesInString(std::string originalString);
        };
}

#endif /* defined(__gufsm__clfsm_machine__) */
