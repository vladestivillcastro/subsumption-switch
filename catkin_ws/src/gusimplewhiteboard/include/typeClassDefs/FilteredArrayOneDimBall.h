
/*
 *  FilteredArrayOneDimBall.h
 *  gusimplewhiteboard
 *
 *  Created by Vlad Estivill-Castro  on 19/06/2014.
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

#ifndef FilteredArrayOneDimBall_DEFINED
#define FilteredArrayOneDimBall_DEFINED

#include <cstdlib>
#include <sstream>
#include <gu_util.h>
#include "FilteredOneDimObject.h"
#include "wb_arrayoffilteredballobjects.h"

namespace guWhiteboard {
    

/**
 * Class for for the array of ball sighthings TOP or Bottom
 */
class FilteredArrayOneDimBall:  public wb_arrayoffilteredballobjects
{
    static const char SEPARATOR_IS_COMMA = ',';
    static const char SEPARATOR_IS_COLON = ':';
    static const char TOP_ID = 'T';
    static const char BOTTM_ID = 'B';
    
    static const char IS_VISIBLE_ID = 'I';
    
public:
    /** designated constructor */
    FilteredArrayOneDimBall(): wb_arrayoffilteredballobjects()
    { /*  */ }
    
    /** constructor with one setter */
    FilteredArrayOneDimBall(const class FilteredOneDimObject &obj, enum FilteredBallObjectType landmarkType  = FVOBallTop)
    {
        _objects[landmarkType]=obj;
    }
    

    
    /** copy constructor */
    FilteredArrayOneDimBall(const FilteredArrayOneDimBall &other)
    {
#ifdef DEBUG
        assert(sizeof(*this) == sizeof(wb_arrayoffilteredballobjects));
#endif
        memcpy(this, &other, sizeof(other));
    }
    
    /** copy assignment operator **/
    FilteredArrayOneDimBall &operator=(const FilteredArrayOneDimBall &other)
    {
        memcpy(this, &other, sizeof(other));
        
        return *this;
    }
    
    /** property getter */
    //class FilteredArrayOneDimSonar *objects() { return _objects; }
    
    
    /** property setter */
    void set_objects(const class FilteredArrayOneDimBall *objects)
    {
        memcpy(_objects, objects, sizeof(_objects));
    }
    
    /** single vision object setter */
    void set_object(const class FilteredOneDimObject &obj, enum FilteredBallObjectType landmarkType  = FVOBallTop)
    {
        wb_arrayoffilteredballobjects::set_objects(obj, landmarkType);
    }
    
    /** single vision object setter */
    FilteredOneDimObject  get_object( enum FilteredBallObjectType landmarkType  = FVOBallTop)
    {
        return objects(landmarkType);
    }

#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
        
        /** string constructor */
        FilteredArrayOneDimBall(const std::string &names) { from_string(names); }
        
        /** const char * constructor */
        FilteredArrayOneDimBall(const char *names) { from_string(names); }

    
    /** convert to a string; string serialization */
    
    std::string description()
    {
        std::ostringstream ss;
        for ( int i =FVOBallTop; i< FVO_NUM_CAMERAS; i++ )
        {
            FilteredBallObjectType landmarkType = FilteredBallObjectType(i);
            switch (landmarkType )
            {
                case FVOBallTop : ss << TOP_ID<<SEPARATOR_IS_COLON;
                    break;
                case FVOBallBottom : ss << BOTTM_ID<<SEPARATOR_IS_COLON ;
                    break;
                case FVO_NUM_CAMERAS : mipal_warn( "ERROR:");
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
        
        std::string top (1,TOP_ID); top+=SEPARATOR_IS_COLON;
        std::string bottom (1,BOTTM_ID); bottom+=SEPARATOR_IS_COLON;
        
        std::size_t found = str.find(top);
        if (std::string::npos!=found )
        {  std::string strFortop=str.substr (found+top.size());
            FilteredOneDimObject thetop(strFortop);
            set_object(thetop,FVOBallTop);
        }
        
        found = str.find(bottom);
        if (std::string::npos!=found )
        {  std::string strForbottom=str.substr (found+bottom.size());
            FilteredOneDimObject thebottom(strForbottom);
            set_object(thebottom,FVOBallBottom );
        }

    }
#endif // WHITEBOARD_POSTER_STRING_CONVERSION

    
    
};
} // namespace


#endif //FilteredArrayOneDimBall
