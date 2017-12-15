/*
 *     $Id: a4a06778c7832c8b027577b409efe55adf44bf46 $
 *
 *  gu_util.h
 *  GUNao
 *
 *  Created by Rene Hexel on 24/04/10.
 *  Copyright 2010-2013 Rene Hexel. All rights reserved.
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
#ifndef _GU_UTIL_H_
#define _GU_UTIL_H_

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#pragma clang diagnostic ignored "-Wc++11-long-long"

#ifdef	DEBUG
#ifndef DBG
#define	DBG(x)		x
#endif
#ifndef	DPRINTF
#define	DPRINTF(x)	printf x
#endif
#else
#ifndef DBG
#define	DBG(x)
#endif
#ifndef	DPRINTF
#define	DPRINTF(x)
#endif
#endif

#define protected_msleep(ms)    protected_usleep(1000LL*(ms))
#define protected_sleep(s)      protected_usleep(1000000LL*(s))

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-macros"
#undef __block
#define __block _xblock
#ifdef __cplusplus
#include <vector>
#include <string>
#include <cstring>
#include <cmath>
#else
#include <math.h>
#include <string.h>
#endif
#undef __block
#define __block __attribute__((__blocks__(byref)))
#pragma clang diagnostic pop

#ifdef __cplusplus

#ifdef bool
#undef bool
#endif

#ifdef true
#undef true
#undef false
#endif

#define atoi16(s)   static_cast<int16_t>(atoi(s))
#define atou16(s)   static_cast<uint16_t>(atoi(s))
#define atoi32(s)   static_cast<int32_t>(atoi(s))
#define atou32(s)   static_cast<uint32_t>(atoi(s))
#define atoi64(s)   static_cast<int64_t>(atoll(s))
#define atou64(s)   static_cast<uint64_t>(atoll(s))

#define PROPERTY_DECLARATION(t, n) protected: t _##n;
#define PROPERTY_GETTER(t, n) public: t &n () { return _##n ; } const t &n () const { return _##n ; }
#define PROPERTY_SETTER(t, n) public: void set_##n (const t &n ) { _##n = n ; }
#define PROPERTY(type, name) PROPERTY_DECLARATION(type,name) PROPERTY_GETTER(type,name) PROPERTY_SETTER(type,name)

#define BIT_DECLARATION(n) protected: unsigned _##n : 1;
#define BIT_GETTER(n) public: unsigned n () const { return _##n ; }
#define BIT_SETTER(n) public: void set_##n (const unsigned n = true) { _##n = n ; }
#define BIT_PROPERTY(name) BIT_DECLARATION(name) BIT_GETTER(name) BIT_SETTER(name)

#define ARRAY_PROPERTY_DECLARATION(t, n, s) protected: t _##n[s];
#define ARRAY_PROPERTY_GETTER(t, n) public: t &n (int e) { return _##n[e] ; } const t &n (int e) const { return _##n[e] ; }
#define ARRAY_PROPERTY_SETTER(t, n) public: void set_##n (const t &n, int e ) { _##n[e] = n ; }
#define ARRAY_PROPERTY(type, name, size) ARRAY_PROPERTY_DECLARATION(type,name,size) ARRAY_PROPERTY_GETTER(type,name) ARRAY_PROPERTY_SETTER(type,name)


#define CONTROLLED_SETTER(t, n) public: void set_##n (const t &n ) { _##n = n ; _##n##_mask = 1;}
#define CONTROLLED_PROPERTY(type, name) PROPERTY_DECLARATION(type, name) PROPERTY_GETTER(type, name) CONTROLLED_SETTER(type, name)

#define CONTROL_BIT(name) BIT_DECLARATION(name##_mask) BIT_GETTER(name##_mask) //these should all be declared together to avoid excess padding

#define VERSION_DECLARATION(n) private: int _##n##_version; static int* get_##n##_classVersion() { static int _##n##_classVersion = 0; return &_##n##_classVersion;}
#define VERSION_PROPERTY_SETTER(t, n) public: void set_##n (const t &n ) { _##n = n; (*get_##n##_classVersion())+=1; _##n##_version = *get_##n##_classVersion();} private: void reset_##n##_version() {(*get_##n##_classVersion())=0;}
#define VERSION_PROPERTY_GETTER(t, n) public: t &n () {(*get_##n##_classVersion()) = _##n##_version; return _##n ; }
#define VERSION_PROPERTY_CHECK(n) public: bool new_##n () {if(_##n##_version > (*get_##n##_classVersion())) return true; return false;}
#define VERSION_PROPERTY_INITIALIZATION(n) _##n##_version = 0;
#define VERSIONED_PROPERTY(type, name) PROPERTY_DECLARATION(type,name) VERSION_DECLARATION(name) VERSION_PROPERTY_SETTER(type,name) VERSION_PROPERTY_GETTER(type,name) VERSION_PROPERTY_CHECK(name)

#define SET_CONTROL_VAL(o,a,v) do { (o).set_##a(v); (o)set_##a##_mask(true); } while (0)

#define CHECK_ENUM(s,p,v)       if (strstr((s), #v)) set_##p(v)
#define STREAM_ENUM_CASE(s,v)   case v: s << #v; break




#define DEG_OVER_RAD (180 / M_PI)
#define RAD_OVER_DEG (M_PI / 180)
#define RAD2DEG(x)					((x) * DEG_OVER_RAD)
#define DEG2RAD(x)					((x) * RAD_OVER_DEG)



extern "C"
{
#else // !__cplusplus
#include <string.h>
#include <stdbool.h>

#define PROPERTY_DECLARATION(t, n) t _##n;
#define PROPERTY_GETTER(t, n)
#define PROPERTY_SETTER(t, n)
#define PROPERTY(type, name) PROPERTY_DECLARATION(type,name)

#define ARRAY_PROPERTY_DECLARATION(t, n, s) t _##n[s];
#define ARRAY_PROPERTY_GETTER(t, n)
#define ARRAY_PROPERTY_SETTER(t, n)
#define ARRAY_PROPERTY(type, name, size) ARRAY_PROPERTY_DECLARATION(type,name,size)
	
#define VERSION_DECLARATION(n) int _##n##_version = 0; static int* get_##n##_classVersion() { static int _##n##_classVersion = 0; return &_##n##_classVersion;}
#define VERSION_PROPERTY_SETTER(t, n)
#define VERSION_PROPERTY_GETTER(t, n)
#define VERSION_PROPERTY_CHECK(n)
#define VERSIONED_PROPERTY(type, name) PROPERTY_DECLARATION(type,name) VERSION_DECLARATION(name) VERSION_PROPERTY_SETTER(type,name) VERSION_PROPERTY_GETTER(type,name) VERSION_PROPERTY_CHECK(name)
#define VERSION_PROPERTY_INITIALIZATION(n) _##n##_version = 0;

#define BIT_DECLARATION(n) unsigned _##n : 1;
#define BIT_GETTER(n) 
#define BIT_SETTER(n)
#define BIT_PROPERTY(name) BIT_DECLARATION(name) BIT_GETTER(name) BIT_SETTER(name)

#endif // __cplusplus

#include <sys/param.h>

#if defined(BSD) && (BSD >= 199506)
#define gu_strlcpy(d,s,n)       strlcpy((d),(s),(n))
#define gu_strlcat(d,s,n)       strlcat((d),(s),(n))
#else
size_t gu_strlcpy(char *dst, const char *src, size_t size);
size_t gu_strlcat(char *dst, const char *src, size_t size);
#endif

/** Return whether a file exists
 * @param fileName	Name of the file to test
 * @return true if the file exists, false otherwise
 */
bool file_exists(const char *fileName);

/** Load the contents of a given file into a string
 * @param fileName	Name of the file to load
 * @return string with contens of file
 */
char *new_string_from_file(const char *fileName);

/** Load the contents of a given file into an int
 * @param fileName	Name of the file to load
 * @return int with the contents of the file
 */
int int_from_file(const char *fileName);

/** Load the contents of a given file into a double
 * @param fileName	Name of the file to load
 * @return double with the contents of the file
 */
double double_from_file(const char *fileName);

/** Duplicate a string
 * @param str	  original string
 * @return char * duplicated string (needs to be free()d)
 */
char *gu_strdup(const char *str);

/** Create a new string by concatenating two path components
 * @param head	  parent directory
 * @param tail	  child path component
 * @return char * with the concatenated path (needs to be free()d)
 */
char *concatenate_path(const char *head, const char *tail);

/** Get the current time of day in microseconds
 * @return long long    with the current time in us (-1LL in case of error)
 */
long long get_utime(void);

/** Protected usleep() -- guaranteed to sleep for the given time!
 * param us   number of microseconds to sleep
 */
void protected_usleep(long long us);
        
/** Return a trimmed copy of the current space by removing all
 *  leading and trailing whitespace characters
 * @param s	  the string to trim
 * @return new string void of leading
 */
const char *gu_strtrim(const char *s);

/** Set the log file for the current module
 * @param filename      name of the file to open.  If the file contains a
 *                      slash '/', it will create the directory the file
 *                      should go in with mode 01777 if non-existent
 * @return -1 in case of errors, 0 in case of success
 */
int mipal_err_file(const char *filename);

/** Print a warning to the file mipal_err_file() has been set for
 *  (or stderr if none has been set)
 * @param fmt      printf() style format string
 */
void mipal_warn(const char *fmt, ...);

/** get the basename of the current program
 * @return const char *  program name
 */
const char *gu_getprogname();
    
/** get player number
 * @return int player number
 */
int getplayernumber();

#ifdef __cplusplus
} // extern "C"

/** Load the contents of a given file into a string
 * @param fileName	Name of the file to load
 * @return string with contens of file
 */
std::string string_from_file(const char *fileName);


/** Concatenate two path components into a new string
 * @param head	  parent directory
 * @param tail	  child path component
 * @return string with the concatenated path
 */
std::string string_by_concatenating_path_components(const std::string &head, const std::string &tail);

/** Trim whitespace characters from both ends of a string.
 * This operation is in-place, so the original string gets modified!
 * @param s	  the string to trim (in place)
 * @return reference to s
 */
std::string &gu_trim(std::string &s);

/** Convert a long value to a string
 * @param val	Value to convert
 * @return string with contens value
 */
std::string gu_ltos(long val);

/** Convert an unsigned long value to a string
 * @param val	Value to convert
 * @return string with contens value
 */
std::string gu_ultos(unsigned long val);

/** Convert a double value to a string
 * @param val	Value to convert
 * @return string with contens value
 */
std::string gu_dtos(double val);

/** Split a string based on the token
 * @param str   The string to split
 * @param sep   Token separator character
 * @param trim  Boolean to specify whether the result should be trimmed
 * @return array of strings split by token
 */
std::vector<std::string> components_of_string_separated(const std::string &str, char sep = '\n', bool trim = false);

/** Increment a number
 * Used to increment an enum value without warnings
 * @param num A pointer to the number to increment
 * @return the new value of the number
 */
int inc(void *num);

/** Decrement a number
 * Used to decrement a enum value without warnings
 * @param num A pointer to the number to decrement 
 * @return the new value of the number
 */
int dec(void *num);

#endif // __cplusplus

#pragma clang diagnostic pop

#endif // _GU_UTIL_H_
