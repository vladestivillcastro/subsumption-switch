/*
 *  Point2D.h
 *  gusimplewhiteboard
 *
 *  Created by Rene Hexel on 25/03/13.
 *  Copyright (c) 2013, 2014 Rene Hexel. All rights reserved.
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


/****************** I M P O R T A N T   */
/* is <class_name>_DEFINED              */
/***************************************/

#ifndef Point2D_DEFINED
#define Point2D_DEFINED

#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
#include <cstdlib>
#include <sstream>
#endif
#include <gu_util.h>
#include "wb_point.h"

namespace guWhiteboard
{

        /**
         * Class for for demonstrating OO-messages.
         */
        class Point2D: public wb_point2d
        {
        public:
            /** designated constructor */
            Point2D(int16_t x = 0, int16_t y = 0): wb_point2d(x,y)  { /* better than set_x(x); set_y(y) */ }

            /** copy constructor */
            Point2D(const Point2D &other): wb_point2d(other.x(), other.y()) {}

            /** copy assignment operator */
            Point2D &operator=(const Point2D &other) { set_x(other.x()); set_y(other.y()); return *this; }

#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
            /** string constructor */
            Point2D(const std::string &names) { from_string(names); }

            /** const char *constructor */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat"
            Point2D(const char *names): Point2D(std::string(names)) { }
#pragma clang diagnostic pop
            /** convert to a string */
            std::string description()
            {
                std::ostringstream ss;
                ss << x() << "," << y();
                return ss.str();
            }

            /** convert from a string */
            void from_string(const std::string &str)
            {
                std::istringstream iss(str);
                std::string token;
                if (getline(iss, token, ','))
                {
		    set_x(  int16_t(atoi(token.c_str())));
		    set_y(0);
                    if (getline(iss, token, ','))
                    {
		        set_y(int16_t(atoi(token.c_str())));
                    }
                }
            }
#endif // WHITEBOARD_POSTER_STRING_CONVERSION
        };
}


#endif // Point2D_DEFINED
