
/**
 * \file  wb_filteredvisionobject.h
 *  gusimplewhiteboard
 *
 *  Created by
 *  \author Vlad Estivill-Castro on
 *  \date 18/06/2014.
 *  Copyright 2014 Vlad Estivill-Castro. All rights reserved.
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
 *    software must display the following acknowledgment:
 *
 *        This product includes software developed by Vlad Estivill-Castro.
 *
 * 4. Neither the name of the author nor the names of contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
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

#ifndef gusimplewhiteboard_wb_filteredvisionobject_h
#define gusimplewhiteboard_wb_filteredvisionobject_h


#include <gu_util.h>

/**
 * Simple filtered informaiton (of a sonar object) ifor the whiteboard
 * One of this is reported for each camera, but we place
 * a Boolean to indicate it was also visible in the other camera
 */
struct wb_filteredvisionobject
{
        PROPERTY(uint64_t, visibilityHistory)   ///< a 64-bit history of whether vision said visible 1 or not visible 0, visibilityHistory&0X1 is the most recent frame
        PROPERTY(int32_t, frameCounter)         ///< frame counter
        PROPERTY(int16_t, distance)             ///< distance to landmark in cm
        PROPERTY(int16_t, x)                    ///< centre x-coordinate in image
        PROPERTY(int16_t, y)                    ///< centre y-coordinate in image
        PROPERTY(int16_t, yaw)                  ///< the Yaw in Degress when the object was last used to generated filtered values
        PROPERTY(bool, isVisible)               ///< is this a credible sighting
        PROPERTY(bool, pad1)                    ///< 16 bit padding
        PROPERTY(int16_t, pad2)                 ///< 32 bit padding

#ifdef __cplusplus
        wb_filteredvisionobject(int32_t frameCounter=0,
                                int16_t distance =0,
                                int16_t x=0,
                                int16_t y=0,
                                int16_t yaw=0,
                               bool isVisible= false,
			       uint64_t visibilityHistory=0
                                ):
        _visibilityHistory(visibilityHistory),
        _frameCounter(frameCounter),
        _distance(distance),
        _x(x),
        _y(y),
        _yaw(yaw),
        _isVisible(isVisible)
        {}
#endif
};


#endif
