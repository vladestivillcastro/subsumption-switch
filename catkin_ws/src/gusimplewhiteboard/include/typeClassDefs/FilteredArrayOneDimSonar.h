
/*
 *  FilteredArrayOneDimSonar.h
 *  gusimplewhiteboard
 *
 *  Created by Vlad Estivill-Castro  on 18/06/2014..
 *  Copyright (c) 2013, 2014 Vlad Estivill-Castro. All rights reserved.
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

#ifndef FilteredArrayOneDimSonar_DEFINED
#define FilteredArrayOneDimSonar_DEFINED

#include <cstdlib>
#include <sstream>
#include <gu_util.h>

#include "typeClassDefs/FilteredOneDimSonar.h"

#include "wb_arrayoffilteredsonarobjects.h"

namespace guWhiteboard {
/**
 * Class for for the array of sonar messages, LEFT and RIGHT.
 */
class FilteredArrayOneDimSonar:  public wb_arrayoffilteredsonarobjects
{
        static const char SEPARATOR_IS_COMMA = ',';
        static const char SEPARATOR_IS_COLON = ':';
        static const char LEFT_ID = 'L';
        static const char RIGHT_ID = 'R';
        
        static const char IS_VISIBLE_ID = 'I';
        
public:
        /** designated constructor */
        FilteredArrayOneDimSonar(): wb_arrayoffilteredsonarobjects()
        { /*  */ }
        
        /** constructor with one setter */
        FilteredArrayOneDimSonar(const class FilteredOneDimSonar &obj, enum FilteredSonarObjectType landmarkType  = FSLeft)
        {
                _objects[landmarkType]=obj;
        }
        

        
        /** copy constructor */
        FilteredArrayOneDimSonar(const FilteredArrayOneDimSonar &other)
        {
#ifdef DEBUG
                assert(sizeof(*this) == sizeof(wb_arrayoffilteredsonarobjects));
#endif
                memcpy(this, &other, sizeof(other));
        }
        
        /** copy assignment operator **/
        FilteredArrayOneDimSonar &operator=(const FilteredArrayOneDimSonar &other)
        {
                memcpy(this, &other, sizeof(other));
                
                return *this;
        }
        
        /** property getter */
        //class FilteredArrayOneDimSonar *objects() { return _objects; }
        
    
      /** property setter */
        void set_objects(const class FilteredArrayOneDimSonar *objects)
        {
                memcpy(_objects, objects, sizeof(_objects));
        }
        
        /** single vision object setter */
        void set_object(const class FilteredOneDimSonar &obj, enum FilteredSonarObjectType landmarkType  = FSLeft)
        {
                wb_arrayoffilteredsonarobjects::set_objects(obj, landmarkType);
        }
        
        /** single vision object setter */
        FilteredOneDimSonar  get_object( enum FilteredSonarObjectType landmarkType  = FSLeft)
        {   
            return objects(landmarkType);
        }
#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
        
        
        /** string constructor */
        FilteredArrayOneDimSonar(const std::string &names) { from_string(names); }
        
        /** const char * constructor */
        FilteredArrayOneDimSonar(const char *names) { from_string(names); }
        
        /** convert to a string; string serialization */
        /* The two elements of the array are printed, distinguished by a starting
         * identification L: (for Left) and R: (for right)
         * Then the string of a FilteredOneDimSonar
         */

        std::string description()
        {
                std::ostringstream ss;
                for ( int i =FSLeft; i< FSO_NUM_OBJECTS; i++ )
                {
                        FilteredSonarObjectType landmarkType = FilteredSonarObjectType(i);
                        switch (landmarkType )
                        {
                            case FSLeft : ss << LEFT_ID<<SEPARATOR_IS_COLON;
                                        break;
                                case FSRight : ss << RIGHT_ID<<SEPARATOR_IS_COLON ;
                                        break;
                                case FSO_NUM_OBJECTS : mipal_warn( "ERROR:");
                                        break;
                        }// switch
                        ss << get_object(landmarkType).description();
                } //for
                return ss.str();
        }
        
        /* build froms tring */
        void from_string(const std::string &str)
        {
                std::istringstream iss(str);
                std::string token;
            
                 std::string left (1,LEFT_ID); left+=SEPARATOR_IS_COLON;
                 std::string right (1,RIGHT_ID); right+=SEPARATOR_IS_COLON;
            
                std::size_t found = str.find(left);
                if (std::string::npos!=found )
                {  std::string strForLeft=str.substr (found+left.size());
                    FilteredOneDimSonar theLeft(strForLeft);
                    set_object(theLeft,FSLeft);
                }
            
            found = str.find(right);
            if (std::string::npos!=found )
            {  std::string strForRight=str.substr (found+right.size());
                FilteredOneDimSonar theRight(strForRight);
                set_object(theRight,FSRight );
            }
            
            
        }
#endif // WHITEBOARD_POSTER_STRING_CONVERSION

        
};
} // namespace


#endif // FilteredArrayOneDimSonar_h
