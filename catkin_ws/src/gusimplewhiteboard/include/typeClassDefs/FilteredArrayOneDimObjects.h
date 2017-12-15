/*
 *  FilteredArrayOneDimObjects.h
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



/****************** I M P O R T A N T   */
/* is <class_name>_DEFINED              */
/***************************************/

#ifndef FilteredArrayOneDimObjects_DEFINED
#define FilteredArrayOneDimObjects_DEFINED

#include <cstdlib>
#include <cassert>
#include <sstream>
#include <gu_util.h>

#include "typeClassDefs/FilteredOneDimObject.h"

#include "wb_arrayoffilteredvisionobjects.h"

namespace guWhiteboard {

/**
 * Class for for the array vision larndmarks (GOAL POSTS)
 */
class FilteredArrayOneDimObjects:  public wb_arrayoffilteredvisionobjects
{
    static const char SEPARATOR_IS_COMMA = ',';
    static const char SEPARATOR_IS_COLON = ':';
    static const char POST_ID = 'P';
    static const char LEFT_POST_ID = 'L';
    static const char RIGHT_POST_ID = 'R';
    static const char CROSS_BAR_ID = 'C';
    
    static const char IS_VISIBLE_ID = 'I';
    
public:
    /** designated constructor */
    FilteredArrayOneDimObjects(): wb_arrayoffilteredvisionobjects()
    { /*  */ }
    
    /** constructor with one setter */
    FilteredArrayOneDimObjects(const class FilteredOneDimObject &obj, enum FilteredVisionObjectType landmarkType  = FVOGoalPostTop)
    {
        _objects[landmarkType]=obj;
    }
    

    
    /** copy constructor */
    FilteredArrayOneDimObjects(const FilteredArrayOneDimObjects &other)
    {
#ifdef DEBUG
        assert(sizeof(*this) == sizeof(wb_arrayoffilteredvisionobjects));
#endif
        memcpy(this, &other, sizeof(other));
    }
    
    /** copy assignment operator **/
    FilteredArrayOneDimObjects &operator=(const FilteredArrayOneDimObjects &other)
    {
        memcpy(this, &other, sizeof(other));
        
        return *this;
    }
    
    
    
    /** property setter */
    void set_objects(const class FilteredArrayOneDimObjects *objects)
    {
        memcpy(_objects, objects, sizeof(_objects));
    }
    
    /** single vision object setter */
    void set_object(const class FilteredOneDimObject &obj, enum FilteredVisionObjectType landmarkType  = FVOGoalPostTop)
    {
        wb_arrayoffilteredvisionobjects::set_objects(obj, landmarkType);
    }
    
    /** single vision object setter */
    FilteredOneDimObject  get_object( enum FilteredVisionObjectType landmarkType  = FVOGoalPostTop)
    {  
        return objects(landmarkType);
    }
    
#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
   
        /** string constructor */
        FilteredArrayOneDimObjects(const std::string &names) { from_string(names); }
        
        /** const char * constructor */
        FilteredArrayOneDimObjects(const char *names) { from_string(names); }
    /** convert to a string; string serialization */
    
    std::string description()
    {
        std::ostringstream ss;
        for ( int i =FVOGoalPostTop; i< FVO_NUM_OBJECTS; i++ )
        {
            FilteredVisionObjectType landmarkType = FilteredVisionObjectType(i);
            switch (landmarkType )
            {
                case FVOGoalPostTop : ss << POST_ID<<SEPARATOR_IS_COLON;
                    break;
                case FVOGoalPostLeftTop : ss << LEFT_POST_ID<<SEPARATOR_IS_COLON ;
                    break;
                case FVOGoalPostRightTop : ss << RIGHT_POST_ID<<SEPARATOR_IS_COLON;
                    break;
                case FVOGoalCrossBarTop : ss << CROSS_BAR_ID <<SEPARATOR_IS_COLON ;
                    break;
//                case FVOGoalPostBottom : ss << POST_ID<<SEPARATOR_IS_COLON;
//                    break;
//                case FVOGoalPostLeftBottom : ss << LEFT_POST_ID<<SEPARATOR_IS_COLON ;
//                    break;
//                case FVOGoalPostRightBottom : ss << RIGHT_POST_ID<<SEPARATOR_IS_COLON;
//                    break;
//                case FVOGoalCrossBarBottom : ss << CROSS_BAR_ID <<SEPARATOR_IS_COLON ;
//                    break;
                case FVO_NUM_OBJECTS : mipal_warn( "ERROR:");
                    break;
            }// switch
            ss << get_object(landmarkType).description();
        } //for
        return ss.str();
    }
    
    /* build froms tring */
    void from_string(const std::string &str)
    {
        using namespace guWhiteboard;
        std::string plain_post (1,POST_ID); plain_post+=SEPARATOR_IS_COLON;
        std::string left (1,LEFT_POST_ID); left+=SEPARATOR_IS_COLON;
        std::string right (1,RIGHT_POST_ID); right+=SEPARATOR_IS_COLON;
        std::string cross_bar (1,CROSS_BAR_ID); cross_bar+=SEPARATOR_IS_COLON;
        
        std::size_t found = str.find(plain_post);
        if (std::string::npos!=found )
        {  std::string strForPlainPost=str.substr (found+plain_post.size());
            FilteredOneDimObject thePlainPost(strForPlainPost);
            set_object(thePlainPost,FVOGoalPostTop);
        }
        
       found = str.find(left);
        if (std::string::npos!=found )
        {  std::string strForLeft=str.substr (found+left.size());
            FilteredOneDimObject theLeft(strForLeft);
            set_object(theLeft,FVOGoalPostLeftTop);
        }
        
        found = str.find(right);
        if (std::string::npos!=found )
        {  std::string strForRight=str.substr (found+right.size());
            FilteredOneDimObject theRight(strForRight);
            set_object(theRight,FVOGoalPostRightTop );
        }
        
        found = str.find(cross_bar);
        if (std::string::npos!=found )
        {  std::string strForBar=str.substr (found+cross_bar.size());
            FilteredOneDimObject theBar(strForBar);
            set_object(theBar,FVOGoalCrossBarTop );
        }
        
        
    }
#endif // WHITEBOARD_POSTER_STRING_CONVERSION
    
    
};

}

#endif // FilteredArrayOneDimObjects
