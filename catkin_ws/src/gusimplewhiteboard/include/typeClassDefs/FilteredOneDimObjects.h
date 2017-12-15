/*
 *  FilteredKalmanObject.h
 *  gusimplewhiteboard / clfsm
 *
 *  Created by Rene Hexel on 25/03/13.
 *  Copyright (c) 2013 Rene Hexel. All rights reserved.
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
#ifndef FilteredOneDimObjects_DEFINED
#define FilteredOneDimObjects_DEFINED
#define FilteredVisionObject_DEFINED
#define FilteredSonarObject_DEFINED
#define FilteredSonarObjects_DEFINED

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"

#include <cstdlib>
#include <sstream>
#include <gu_util.h>
#include "Vision_Control.h"

#define FVOTopBall      FVOGoalPost
#define FVOBottomBall   FVOGoalPostLeft

namespace guWhiteboard
{
        /**
         * Discriminate sigthings 
         */
        enum FilteredVisionObjectType
        {
            FVOGoalPost,          ///< Filtered informaiton for a post we cannot tell is Left or right
            FVOGoalPostLeft,          ///< Filtered informaiton for a post we know is Left 
            FVOGoalPostRight,          ///< Filtered informaiton for a post we know is right 
            FVOGoalCrossBar,          ///< Filtered informaiton for the Ball
            FVO_NUM_OBJECTS          ///< number of different kind of objects
        };


        enum FilteredSonarObjectType
        {
	    FSLeft,
	    FSRight,
            FSO_NUM_OBJECTS          ///< number of different kind SONAR of objects
        };


        /**
         * Class for a single Sonar sigthing.
         */
        class FilteredSonarObject
        {
            PROPERTY(bool, isVisible) //  is this a credible sighting
            PROPERTY(int16_t, distance) //  distance to landmark in cm
            PROPERTY(int32_t, frameCounter) //  frame counter in cm

        public:
            /** designated constructor */
            FilteredSonarObject( bool isVisible = false,
                                       int16_t distance =0,
				       int32_t frameCounter =0
				       ):
                                      _isVisible(isVisible), 
                                      _distance(distance), 
                                      _frameCounter(frameCounter)
                                         { /*  */ }

            /** string constructor */
            FilteredSonarObject(const std::string &names) { from_string(names); }

            /** copy constructor */
            FilteredSonarObject(const FilteredSonarObject &other):
                      _isVisible(other._isVisible),
                      _distance(other._distance),
                      _frameCounter(other._frameCounter)
                       {}

            /** copy assignment operator **/
            FilteredSonarObject &operator=(const FilteredSonarObject &other)
            {
                _isVisible = other._isVisible;
                _distance = other._distance;
                _frameCounter= other._frameCounter;

                return *this;
            }

            /** convert to a string */
            std::string description() const
            {
                std::ostringstream ss;

		if ( isVisible())
                { ss<<" is visible at distance " << distance() <<  " "; }
		else ss << " is NOT visible ";

		ss << " at frame  " << frameCounter();
                return ss.str();
            }

	    //*** TODO: still incomplete */
            void from_string(const std::string &str)
            {
                std::istringstream iss(str);
            }

        };


        /**
         * Class for a single Vision sigthing.
         */
        class FilteredVisionObject
        {
            PROPERTY(bool, isVisibleTop) //  is this a credible sighting
            PROPERTY(bool, isVisibleBottom) //  is this a credible sighting
            PROPERTY(int16_t, distance) //  distance to landmark in cm
            PROPERTY(int32_t, frameCounter) //  frame counter 
            PROPERTY(int16_t, xTop) //  centre x-coordinate in image
            PROPERTY(int16_t, yTop) //  centre y-coordinate in image
            PROPERTY(int16_t, xBottom) //  centre x-coordinate in image
            PROPERTY(int16_t, yBottom) //  centre y-coordinate in image
            PROPERTY(int16_t, yaw) //  the Yaw in Degress when the object was last used to generated filtered values
        public:
            /** designated constructor */
            FilteredVisionObject( bool isVisibleTop = false, bool isVisibleBottom = false,
                                       int16_t distance =0,
				       int32_t frameCounter =0,
                                       int16_t xTop = 0,
				       int16_t yTop = 0,
                                       int16_t xBottom = 0,
				       int16_t yBottom = 0,
				       int16_t yaw=0):
                                      _isVisibleTop(isVisibleTop), 
                                      _isVisibleBottom(isVisibleBottom), 
                                      _distance(distance), 
                                      _frameCounter(frameCounter), 
                                       _xTop(xTop), _yTop(yTop), _xBottom(xBottom), _yBottom(yBottom), _yaw(yaw)  { /* better than set_x(x); set_y(y) */ }

            /** string constructor */
            FilteredVisionObject(const std::string &names) { from_string(names); }

            /** copy constructor */
            FilteredVisionObject(const FilteredVisionObject &other):
                      _isVisibleTop(other._isVisibleTop),
                      _isVisibleBottom(other._isVisibleBottom),
                      _distance(other._distance),
                      _frameCounter(other._frameCounter),
                      _xTop(other._xTop), _yTop(other._yTop),
                      _xBottom(other._xBottom), _yBottom(other._yBottom),
		      _yaw(other._yaw) {}

            /** copy assignment operator */
            FilteredVisionObject &operator=(const FilteredVisionObject &other)
            {
                _isVisibleTop = other._isVisibleTop;
                _isVisibleBottom = other._isVisibleBottom;
                _distance = other._distance;
                _frameCounter = other._frameCounter;
                _xTop = other._xTop; _yTop = other._yTop;
                _xBottom = other._xBottom; _yBottom = other._yBottom;
                _yaw = other._yaw;

                return *this;
            }
            
	    bool isVisible() const { return isVisibleTop() || isVisibleBottom(); }

            /** return the angle of the object as seen from the robot (in radians) */
            float horizontal_angle(const float guvision_width = 1280.0f, const float horiz_fov = 61.0f) const
	    {
		return (horizontal_angleTop(guvision_width, horiz_fov) + horizontal_angleBottom(guvision_width, horiz_fov)) / 2;
	    }

            float horizontal_angleTop(const float guvision_width = 1280.0f, const float horiz_fov = 61.0f) const
            {
                float yaw_in_radians = float(DEG2RAD(yaw()));                       // head yaw in radians
                float D = guvision_width / 2 / sinf(float(DEG2RAD(horiz_fov/2)));   // projection distance in pixel units
                float alpha = atanf(float(xTop())/D);                                  // negative angle of x on screen

                return yaw_in_radians - alpha;
            }

	    int16_t get_x(VisionCamera whichCamera )
	    { if (Top==whichCamera) return xTop();
		    else return xBottom();
	    }

	    int16_t get_y(VisionCamera whichCamera )
	    { if (Top==whichCamera) return yTop();
		    else return yBottom();
	    }

	    void set_x(VisionCamera whichCamera, int16_t value)
	    { if (Top==whichCamera) set_xTop(value);
		    else set_xBottom(value);
	    }

	    void set_y(VisionCamera whichCamera, int16_t value)
	    { if (Top==whichCamera) set_yTop(value);
		    else set_yBottom(value);
	    }

            float horizontal_angleBottom(const float guvision_width = 1280.0f, const float horiz_fov = 61.0f) const
            {
                float yaw_in_radians = float(DEG2RAD(yaw()));                       // head yaw in radians
                float D = guvision_width / 2 / sinf(float(DEG2RAD(horiz_fov/2)));   // projection distance in pixel units
                float alpha = atanf(float(xBottom())/D);                                  // negative angle of x on screen

                return yaw_in_radians - alpha;
            }
#if 0
            /** XXX: this needs pitch on the WB first! */
            float vertical_angle(const float guvision_height = 960.0f, const float vert_fov = 48.0f) const
            {
            }
#endif

            /** convert to a string */
            std::string description() const
            {
                std::ostringstream ss;

		if ( isVisibleTop())
                { ss<<"TopVisible ";
		  ss <<  " ("  <<  xTop() << "," << yTop() << "," << yaw() << ")@" << distance() <<",";
		}
		if ( isVisibleBottom())
                { ss<<"BottomVisible ";
		  ss <<  " ("  <<  xBottom() << "," << yBottom() << "," << yaw() << ")@" << distance() << ",";
		}
                return ss.str();
            }

            /** Only grab the distance x,y positions convert from a string */
	    //*** TODO: still incomplete */
            void from_string(const std::string &str)
            {
                std::istringstream iss(str);

		set_isVisibleBottom(false);
		set_isVisibleBottom(false);
		std::string openP ("(");

		//std::cerr << "Input to parser" << str << std::endl;
                std::string token;
                if (getline(iss, token, ','))
                {  
                    if ('T'==token[0])  // for the top camera
		    {
		           set_isVisibleTop(true);
		           set_xTop(0); set_yTop(0); set_yaw(0); set_distance(0);
			   std::size_t found = str.find(openP);
			   if (std::string::npos!=found )
			            { std::string strCoordinates=str.substr (found+openP.size()); 
				      std::istringstream second_iss(strCoordinates); //string with the Visible
				      if (getline(second_iss, token, ','))
				      { set_xTop(  int16_t(atoi(token.c_str())));
                                        if (getline(iss, token, ','))
                                            { set_yTop(int16_t(atoi(token.c_str())));
                                             if (getline(iss, token, ')'))
                                             { set_yaw(int16_t(atoi(token.c_str())));
                                               if (getline(iss, token, ','))
                                                  { set_distance(int16_t(atoi(token.substr(1,token.length()).c_str())));
		                                    //std::cerr << "token" << token << std::endl;
				                  }
				            }
				           }
				      }
				    }

		    }
		    else // for the bottom camera
		    {
		           set_isVisibleBottom(true);
		           set_xBottom(0); set_yBottom(0); set_yaw(0); set_distance(0);
			   std::size_t found = str.find(openP);
			   if (std::string::npos!=found )
			            { std::string strCoordinates=str.substr (found+openP.size()); 
				      std::istringstream second_iss(strCoordinates); //string with the Visible
				      if (getline(second_iss, token, ','))
				      { set_xBottom(  int16_t(atoi(token.c_str())));
                                        if (getline(iss, token, ','))
                                            { set_yBottom(int16_t(atoi(token.c_str())));
                                             if (getline(iss, token, ')'))
                                             { set_yaw(int16_t(atoi(token.c_str())));
                                               if (getline(iss, token, ','))
                                                  { set_distance(int16_t(atoi(token.substr(1,token.length()).c_str())));
		                                    //std::cerr << "token" << token << std::endl;
				                  }
				            }
				           }
				      }
				    }
		    }
                }
            }

        };

	/**
	 * Filtered vision objects on the whiteboard
	 */
        class FilteredOneDimObjects
	{
 	//ARRAY_PROPERTY(FilteredVisionObject, objects, FVO_NUM_OBJECTS)
	class FilteredVisionObject     _objects[FVO_NUM_OBJECTS];

        public:
	    FilteredOneDimObjects()
	    {}


	    /** single vision object setter */
	    FilteredOneDimObjects(const class FilteredVisionObject &obj, enum FilteredVisionObjectType landmarkType  = FVOGoalPost)
	    {
		    /*
		for ( int i =FVOGoalPost; i< FO_NUM_OBJECTS; i++ )
		{ FilteredVisionObjectType iThLandmarkType = FilteredVisionObjectType(i);
		}
			*/
		_objects[landmarkType]=obj;
	    }

            /** string constructor */
            FilteredOneDimObjects(const std::string &names) { from_string(names); }

            /** copy constructor */
            FilteredOneDimObjects(const FilteredOneDimObjects &other)
	    {
		    memcpy(this, &other, sizeof(other));
	    }

            FilteredOneDimObjects &operator=(const FilteredOneDimObjects &other)
            {
                memcpy(this, &other, sizeof(other));
                return *this;
            }

	    bool isVisible() 
	    { 
		for(int i = 0; i < FVO_NUM_OBJECTS; i++) 
	 		if (_objects[i].isVisible())
				return true; 
		return false;
	    }

	    /** property getter */
	    //REMOVE this. We have an array property #def that includes a getter and setter
	    class FilteredVisionObject *objects() { return _objects; }

	    /** property setter */
	    void set_objects(const class FilteredVisionObject *objects)
	    {
		    memcpy(_objects, objects, sizeof(_objects));
	    }

	    /** single vision object setter */
	    void set_object(const class FilteredVisionObject &obj, enum FilteredVisionObjectType landmarkType  = FVOGoalPost)
	    {
		_objects[landmarkType]=obj;
	    }

	    /** single vision object setter */
	    FilteredVisionObject &get_object( enum FilteredVisionObjectType landmarkType  = FVOGoalPost)
	    {
		return _objects[landmarkType];
	    }

                /** single vision object getter */
                const FilteredVisionObject &get_object( enum FilteredVisionObjectType landmarkType  = FVOGoalPost) const
                {
                        return _objects[landmarkType];
                }
                
            /** convert to a string */
            std::string description() const
            {
                std::ostringstream ss;
		for ( int i =FVOGoalPost; i< FVO_NUM_OBJECTS; i++ )
		{ 
			FilteredVisionObjectType landmarkType = FilteredVisionObjectType(i);
		  switch (landmarkType )
		   {
		    case FVOGoalPost : ss << "aGoalPost:";
			break;
		    case FVOGoalPostLeft : ss << "aLeftGoalPost:";
			break;
		    case FVOGoalPostRight : ss << "aRightGoalPost:";
			break;
		    case FVOGoalCrossBar : ss << "aCrossBar:";
			break;
		    case FVO_NUM_OBJECTS : mipal_warn( "ERROR:");
			break;
		  }// switch
		  ss <<_objects[landmarkType].description();
                        if (i < FVO_NUM_OBJECTS-1)
                                ss << ", ";
		} //for
                return ss.str();
	  }

	    /*
	     * TODO, this is only for the BALL, the axiom from_string() is inverse of description() 
	     * NOT WORKING
	     */
            void from_string(const std::string &str)
            {
                std::istringstream iss(str);
                std::string token;
                for (int object = FVOGoalPost; object < FVO_NUM_OBJECTS; object++)
                {
                    if (!getline(iss, token, '\t')) break;
                    _objects[object].from_string( token );
                }

            }

	};


	/**
	 * Filtered sonar objects on the whiteboard
	 */
        class FilteredSonarObjects
	{
	class FilteredSonarObject     _objects[FSO_NUM_OBJECTS];

        public:
	    FilteredSonarObjects()
	    {}

	    /** constructor with one setter */
	    FilteredSonarObjects(const class FilteredSonarObject &obj, enum FilteredSonarObjectType landmarkType  = FSLeft)
	    {
		_objects[landmarkType]=obj;
	    }

            /** string constructor */
            FilteredSonarObjects(const std::string &names) { from_string(names); }

            /** copy constructor */
            FilteredSonarObjects(const FilteredSonarObjects &other)
	    {
		    memcpy(this, &other, sizeof(other));
	    }

            /** copy assignment operator **/
            FilteredSonarObjects &operator=(const FilteredSonarObjects &other)
            {
                memcpy(this, &other, sizeof(other));

                return *this;
            }

	    /** property getter */
	    class FilteredSonarObject *objects() { return _objects; }

	    /** property setter */
	    void set_objects(const class FilteredSonarObject *objects)
	    {
		    memcpy(_objects, objects, sizeof(_objects));
	    }

	    /** single vision object setter */
	    void set_object(const class FilteredSonarObject &obj, enum FilteredSonarObjectType landmarkType  = FSLeft)
	    {
		_objects[landmarkType]=obj;
	    }

	    /** single vision object setter */
	    FilteredSonarObject  get_object( enum FilteredSonarObjectType landmarkType  = FSLeft)
	    {
		return _objects[landmarkType];
	    }

            /** convert to a string */
            std::string description()
            {
                std::ostringstream ss;
		for ( int i =FSLeft; i< FSO_NUM_OBJECTS; i++ )
		{ 
			FilteredSonarObjectType landmarkType = FilteredSonarObjectType(i);
		  switch (landmarkType )
		   { 
		    case FSLeft : ss << "aLeftSonar:";
			break;
		    case FSRight : ss << "aRightSonar:";
			break;
		    case FSO_NUM_OBJECTS : mipal_warn( "ERROR:");
			break;
		  }// switch
		  ss <<_objects[landmarkType].description();
		} //for
                return ss.str();
	  }

	    /*
	     * TODO, this is only for the BALL, the axiom from_string() is inverse of description() 
	     * NOT WORKING
	     */
            void from_string(const std::string &str)
            {
		  _objects[FVOGoalPost].from_string( str );
            }

	};
}

#pragma clang diagnostic pop

#endif // FilteredSonarObjects_DEFINED
