/*
 *  clfsm_machine.cc
 *  clfsm
 *
 *  Created by Rene Hexel on 11/10/12.
 *  Copyright (c) 2012, 2014 Rene Hexel. All rights reserved.
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
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <dispatch/dispatch.h>
#include <libgen.h>

#ifdef bool
#undef bool
#endif

#ifdef true
#undef true
#undef false
#endif

#include <dlfcn.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <gu_util.h>
#include "clfsm_cc.h"
#include "clfsm_machine.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#pragma clang diagnostic ignored "-Wvla"
#pragma clang diagnostic ignored "-Wvla-extension"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wunused-function"

using namespace std;
using namespace FSM;

static dispatch_queue_t sync_queue = NULL;

MachineWrapper::MachineWrapper(string path): _fullPath(path), _factory(NULL), _shared_object(NULL), _compiler(NULL), _compiler_args(NULL), _linker_args(NULL), _delete_compiler(false)
{
        if (!file_exists(path.c_str()))
        {
                string machinePath = path + ".machine";
                if (file_exists(machinePath.c_str()))
                        path = machinePath;
        }
        char pathName[path.length()];

        strcpy(pathName, path.c_str());
        char *base = basename(pathName);
        char *dot = strchr(base, '.');
        if (dot) *dot = '\0';

        _name = base;
}


MachineWrapper::~MachineWrapper()
{
        if (_shared_object) dlclose(_shared_object);
        if (_delete_compiler) delete _compiler;
}


void MachineWrapper::setCompiler(Cc *compiler, bool del)
{
        if (!compiler)
        {
                compiler = new Cc();
                del = true;
        }
        _compiler = compiler;
        _delete_compiler = del;
}


string MachineWrapper::binaryDirectory() const
{
        struct utsname buffer;
        stringstream ss;

        ss << _fullPath << "/";

        if (uname(&buffer) == -1)
                ss << "unknown-architecture";
        else
                ss << buffer.sysname << "-" << buffer.machine;

        return ss.str();
}

string MachineWrapper::includePathFile() const
{
        stringstream ss;
        ss << _fullPath << "/IncludePath";

        return ss.str();
}

vector<string> MachineWrapper::states() const
{
        stringstream filename;

        filename << path() << "/States";

        const string &fn = filename.str();
        DBG(cout << fn << endl);
        ifstream file(fn.c_str());
        vector<string> states;

        while (!file.eof())
        {
                string name;
                getline(file, name);
                if (file.fail())
                        break;
                if (name.length())
                        states.push_back(name);
        }

        return states;
}

static void create_compile_queue(void *)
{
        sync_queue = dispatch_queue_create("net.mipal.clfsm.compile", 0);
}

struct outfile_pushback_param
{
        vector<string> *outfilesp;
        string *outfilenamep;
};

static void push_back_outfilename(void *p)
{
        outfile_pushback_param *param = static_cast<outfile_pushback_param *>(p);
        param->outfilesp->push_back(*param->outfilenamep);
}


bool MachineWrapper::compile(const vector<string> &compiler_args, const vector<string> &linker_args)
{
        string binary_directory = binaryDirectory();
        static dispatch_once_t onceToken;
        dispatch_once_f(&onceToken, NULL, create_compile_queue);

        if (!compiler()) setCompiler();

        mkdir(binary_directory.c_str(), 0777);

        vector<string> files = states();
        files.push_back("");

#ifdef __BLOCKS__
        __block vector<string> outfiles;
        __block bool success = true;
        dispatch_apply(files.size(), dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(size_t i)
#else
        vector<string> outfiles;
        bool success = true;
        for (size_t i = 0; i < files.size(); i++)
#endif
        {
                vector<string> args = compiler_args;
                stringstream file;
                stringstream outfile;

                if (files[i].length())
                        file << "State_" << files[i];
                else
                        file << name();

                outfile << binary_directory << "/" << file.str() << ".o";
                file << ".mm";

                string outfilename = outfile.str();

                args.push_back("-I");
                args.push_back(path());
                args.push_back("-Weverything");
                args.push_back("-Wno-weak-vtables");
                args.push_back("-Wno-padded");
                args.push_back("-stdlib=libc++");
                args.push_back("-c");
                args.push_back("-o");
                args.push_back(outfilename);
                args.push_back(_fullPath + "/" + file.str());

                if (!compiler()->compile(args))
                        success = false;
#ifdef __BLOCKS__
                else dispatch_sync(sync_queue,
                ^{
                        outfiles.push_back(outfilename);
                });
        });
#else
                else
                {
                        outfile_pushback_param param = { &outfiles, &outfilename };
                        dispatch_sync_f(sync_queue, &param, push_back_outfilename);
                }
        }
#endif
        if (success)    // link into shared object if compiler was successful
        {
                vector<string> args = linker_args;
                for (vector<string>::iterator of = outfiles.begin(); of != outfiles.end(); of++)
                        args.push_back(*of);
                args.push_back("-shared");
                args.push_back("-stdlib=libc++");
                args.push_back("-lclfsm");
                args.push_back("-o");
                args.push_back(binary_directory + "/" + name() + ".so");

                success = compiler()->compile(args);
        }

        return success;
}


CLMachine *MachineWrapper::instantiate(int id, const char *machine_name)
{
        if (!_shared_object)
        {
                string shared_path = binaryDirectory() + "/" + _name + ".so";
                if (!(_shared_object = dlopen(shared_path.c_str(), RTLD_NOW|RTLD_GLOBAL)))
                {
                        const char *error = dlerror();
                        if (error) cerr << error << endl;

                        const vector<string> *cmdline_compiler_args = _compiler_args;
                        const vector<string> *linker_args = _linker_args;
                        vector<string> compiler_args = cmdline_compiler_args ? *cmdline_compiler_args : default_compiler_args();

                        add_machine_includes(compiler_args);

                        if (!linker_args)   linker_args   = &default_linker_args();

                        compile(compiler_args, *linker_args);

                        if (!(_shared_object = dlopen(shared_path.c_str(), RTLD_NOW|RTLD_GLOBAL)))
                        {
                                if (!error)
                                {
                                        error = dlerror();
                                        if (error) cerr << error << endl;
                                        else cerr << "Unkown error!" << endl;
                                }
                                return NULL;
                        }
                }
        }
        if (!_factory)
        {
                string symbol = string("CLM_Create_") + name();
                _factory = create_machine_f(dlsym(_shared_object, symbol.c_str()));
                if (!_factory)
                {
                        symbol = string("_CLM_Create_") + name();
                        if (!(_factory = create_machine_f(dlsym(_shared_object, symbol.c_str()))))
                                return NULL;
                }
        }
        return _factory(id, machine_name);
}


string MachineWrapper::stringByExpandingEnvironmentVariablesInString(string originalString)
{
        string expandedString;
        size_t start = 0;
        size_t pos = originalString.find_first_of('$');
        while (pos != string::npos)
        {
                const char *value;
                string env;
                size_t end;
                if (pos > start)
                        expandedString += originalString.substr(start, pos-start);
                if (originalString[++pos] == '{' && (end = originalString.find_first_of('}', pos+1)) != string::npos)
                {
                        ++pos;
                        env = originalString.substr(pos, end++-pos);
                        start=end;
                }
                else if ((end = originalString.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJLKMNOPQRSTUVWXYZ0123456789_", pos)) != string::npos)
                {
                        env = originalString.substr(pos, end-pos);
                        start = end;
                }
                else
                {
                        if (originalString[pos] == '$') pos++;
                        start = pos;
                        goto skip_converting;
                }
                if ((value = getenv(env.c_str()))) expandedString += value;

        skip_converting:
                pos = originalString.find_first_of('$', start);
        }
        if (start < originalString.length())
                expandedString += originalString.substr(start);

        return expandedString;
}


void MachineWrapper::add_machine_includes(vector<string> &compiler_args)
{
        string fn = includePathFile();
        DBG(cout << fn << endl);
        ifstream file(fn.c_str());

        while (!file.eof())
        {
                string directory;
                getline(file, directory);
                if (file.fail())
                        break;
                if (directory.length())
                        compiler_args.push_back(string("-I")+stringByExpandingEnvironmentVariablesInString(directory));
        }
}


const vector<string> &MachineWrapper::default_compiler_args()
{
        static vector<string> args;

        if (!args.size())
        {
                args.push_back("-I/usr/local/include/clfsm");
                args.push_back("-I/usr/local/include");
        }
        return args;
}


const vector<string> &MachineWrapper::default_linker_args()
{
        static vector<string> args;

        if (!args.size())
        {
                args.push_back("-L/usr/local/lib");
                args.push_back("-rpath");
                args.push_back("/usr/local/lib");
        }
        return args;
}

#pragma clang diagnostic pop
