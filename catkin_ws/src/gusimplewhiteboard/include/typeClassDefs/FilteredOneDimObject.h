
/*
 *  FilteredOneDimObject.h
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

#ifndef FilteredOneDimObject_DEFINED
#define FilteredOneDimObject_DEFINED

#include <cstdlib>
#include <sstream>
#include <gu_util.h>
#include "wb_filteredvisionobject.h"

namespace guWhiteboard {
class FilteredOneDimObject:  public wb_filteredvisionobject
{

        
public:
        /** designated constructor */
        FilteredOneDimObject(int32_t frameCounter=0,
                             int16_t distance =0,
                             int16_t x=0,
                             int16_t y=0,
                             int16_t yaw=0,
                                                       bool isVisible= false,
                             uint64_t visibilityHistory=0
                            ): wb_filteredvisionobject(frameCounter,distance,x,y,yaw,isVisible,visibilityHistory)
        { /*  */ }
        

        
        /** copy constructor */
        FilteredOneDimObject(const FilteredOneDimObject &other):wb_filteredvisionobject(other.frameCounter(), other.distance() ,other.x(),other.y(),other.yaw(),other.isVisible(), other.visibilityHistory())
        {
        }
    
         /** BASECONSTRUCTOR: INTERESTING !!!! */
         FilteredOneDimObject (const wb_filteredvisionobject &other)
       {    set_isVisible ( other.isVisible() );
        
        set_frameCounter ( other.frameCounter() );
           set_distance ( other.distance() );
            set_x ( other.x() );
            set_y ( other.y() );
             set_yaw ( other.yaw() );
           set_isVisible ( other.isVisible() );
               set_visibilityHistory ( other.visibilityHistory() );

        
    }
    
        /** copy assignment operator **/
        FilteredOneDimObject &operator=(const FilteredOneDimObject &other)
        {
                set_frameCounter ( other.frameCounter() );
                set_distance ( other.distance() );
                set_x ( other.x() );
                set_y ( other.y() );
                set_yaw ( other.yaw() );
                set_isVisible ( other.isVisible() );
                set_visibilityHistory ( other.visibilityHistory() );

                return *this;
        }
    
    /** copy assignment operator **/
    /**  INTERESTING !!!! */
    FilteredOneDimObject &operator=(const wb_filteredvisionobject &other)
    {
        set_frameCounter ( other.frameCounter() );
        set_distance ( other.distance() );
        set_x ( other.x() );
        set_y ( other.y() );
        set_yaw ( other.yaw() );
        set_isVisible ( other.isVisible() );
        set_visibilityHistory ( other.visibilityHistory() );
        
        return *this;
    }
    
        /********************* Useful utilities *****************************/
        float horizontal_angle(const float guvision_width = 640.0f, const float horiz_fov = 61.0f) const
        {
                float yaw_in_radians = float(DEG2RAD(yaw()));                       // head yaw in radians
                float D = guvision_width / 2 / tanf(float(DEG2RAD(horiz_fov/2)));   // projection distance in pixel units
                float alpha = atanf(float(x())/D);                                  // negative angle of x on screen
                
                return yaw_in_radians - alpha;
        }
        
        int ratioOfSightings(const int length=64) const{  // # of object sigthings over length
                int historyLength = (length>64) ? 64 : length;
                historyLength = (historyLength>0)?historyLength:64;
                uint64_t theHistory=visibilityHistory();
                
                int count=0;
                int position=historyLength;
                
                while (position>0)
                { count +=theHistory & 0X1;
                        theHistory >>=1;
                        position--;
                }
                
                return count;
        }
        
#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
        
        static const char SEPARATOR_IS_COMMA = ',';
        static const char SEPARATOR_IS_COLON = ':';
        
        static const char IS_VISIBLE_ID = 'I';
        /** string constructor */
        FilteredOneDimObject(const std::string &names) { from_string(names); }
        
        /** const char * constructor */
        FilteredOneDimObject(const char *names) { from_string(names); }

        /** convert to a string */
        // WARNING we do not convert the visionHisory toa nd from the String
        std::string description() const
        {
                std::ostringstream ss;
                
                if ( isVisible())
                { ss<<"ISvisible"<< SEPARATOR_IS_COMMA << distance() <<  SEPARATOR_IS_COMMA; }
                else ss << "NOTvisible"<< SEPARATOR_IS_COMMA <<  distance() <<  SEPARATOR_IS_COMMA;
                
                ss << x() << SEPARATOR_IS_COMMA << y() << SEPARATOR_IS_COMMA << yaw() <<  SEPARATOR_IS_COMMA;
                ss << "FRAME" << SEPARATOR_IS_COLON << frameCounter() <<  SEPARATOR_IS_COMMA;
                ss << "Sightings" << SEPARATOR_IS_COLON << ratioOfSightings(64)*100.0f/64.0f <<  SEPARATOR_IS_COMMA; //Not in the from_string parser yet
                return ss.str();
        }
        
        //*** TODO: still incomplete */
        void from_string(const std::string &str)
        {       std::string colonDel (1,SEPARATOR_IS_COLON);
                 std::string comaDel (1,SEPARATOR_IS_COMMA);
                set_isVisible ( false );
                
                std::istringstream iss(str);
                std::string token;
                if (getline(iss, token, SEPARATOR_IS_COMMA))
                { if (IS_VISIBLE_ID==token[0])
                { set_isVisible ( true );
                        set_distance ( 0 );
                        set_x(0);
                        set_y(0);
                        set_yaw(0);
                        set_frameCounter ( 0 );
                       
                        std::size_t found = str.find(comaDel);
                        if (std::string::npos!=found )
                        { std::string strSecond=str.substr (found+comaDel.size());
                                std::istringstream second_iss(strSecond);
                                if (getline(second_iss, token, SEPARATOR_IS_COMMA))
                                {
                                        int16_t distance_value = int16_t ( atoi(token.c_str()));
                                        set_distance ( distance_value>=0? distance_value : -distance_value);
                                    // after the x
                                        found = strSecond.find(comaDel);
                                        if (std::string::npos!=found )
                                        { std::string strThird=strSecond.substr (found+comaDel.size());
                                          std::istringstream third_iss(strThird);
                                                if (getline(third_iss, token, SEPARATOR_IS_COMMA))
                                                {
                                                        int16_t xValue=int16_t ( atoi(token.c_str()));
                                                        set_x (xValue);
                                                         // after the y
                                                        found = strThird.find(comaDel);
                                                         if (std::string::npos!=found )
                                                         { std::string strFourth=strThird.substr (found+comaDel.size());
                                                           std::istringstream fourth_iss(strFourth);
                                                           if (getline(fourth_iss, token, SEPARATOR_IS_COMMA))
                                                           { int16_t yValue=int16_t ( atoi(token.c_str()));
                                                              set_y (yValue);
                                                              // after the yaw
                                                              found = strFourth.find(comaDel);
                                                              if (std::string::npos!=found )
                                                              { std::string strFifith=strFourth.substr (found+comaDel.size());
                                                                std::istringstream fifith_iss(strFifith);
                                                                if (getline(fifith_iss, token, SEPARATOR_IS_COMMA))
                                                                { int16_t yawValue=int16_t ( atoi(token.c_str()));
                                                                   set_yaw (yawValue);
                                                                        /* after the frame */
                                                                   
                                                                    found = strFifith.find(colonDel);
                                                                    if (std::string::npos!=found )
                                                                   { std::string strSixth=strFifith.substr (found+colonDel.size());
                                                                     std::istringstream sixth_iss(strSixth);
                                                                     if (getline(sixth_iss, token, SEPARATOR_IS_COMMA))
                                                                                {
                                                                                        int32_t frameValue=int32_t ( atoi(token.c_str()));
                                                                                        set_frameCounter (frameValue);
                                                                                        
                                                                                }
                                                                   }

                                                                        
                                                                }
                                                                      
                                                              }
                                                                   
                                                           }
                                                                 
                                                         }
                                                        
                                                }
                                                
                                                
                                        }
                                        
                                }
                                
                        }
                        
                        
                }
                else
                { *this=FilteredOneDimObject();     }
                        
                        
                        
                }
                else
                {
                        *this=FilteredOneDimObject();
                }
        }
        
#endif // WHITEBOARD_POSTER_STRING_CONVERSION

        
};
} //namespace


#endif //FilteredOneDimObject
