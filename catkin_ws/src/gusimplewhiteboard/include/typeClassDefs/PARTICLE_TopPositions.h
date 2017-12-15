/*
 *  PARTICLE_TopPositions.h
 *  gugridparticlefilter / clfsm
 *
 *  Created by Rene Hexel on 4/06/13.
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
#ifndef PARTICLE_TopPositions_DEFINED
#define PARTICLE_TopPositions_DEFINED
#define TopParticles_DEFINED

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"

#include <cstdlib>
#include <sstream>
#include <gu_util.h>
#include "Point2D.h"

#define NUM_TOP_PARTICLES       4

namespace guWhiteboard
{
        
        /**
         * command for Particle Status
         */
        enum PF_ControlStatus_Modes
        {
                PF_Running,             ///< PF is running
                PF_New_Game,      ///< A FSM should set this, the PF is reset as per playerNumber
                PF_After_Penalized,              ///< Sides of our field are the reset positions
                PF_After_Fall,             ///< headings are reset randomly
                PF_Manual_Placement          ///< maybe after 34 seconds and manual placelemnt
           
        };
        
        
        /**
         * Class for a top particle position
         */
        class ParticlePosition: public Point2D
        {
                PROPERTY(int16_t, headingInDegrees)     ///< heading in degrees
                PROPERTY(float, confidence)             ///< total confidence
        public:
                /** designated constructor */
                ParticlePosition(int16_t x = 0, int16_t y = 0, int16_t degrees = 0, float weight = 0.0): Point2D(x, y), _headingInDegrees(degrees), _confidence(weight) {}

                /** copy constructor */
                ParticlePosition(const ParticlePosition &other): Point2D(other), _headingInDegrees(other._headingInDegrees), _confidence(other._confidence) { }

                /** copy assignment operator */
                ParticlePosition &operator=(const ParticlePosition &other) { _headingInDegrees=other._headingInDegrees; _confidence=other._confidence; return *this; }

                /** radians getter */
                float heading() const { return float(DEG2RAD(headingInDegrees())); }

                /** radians setter */
                void set_heading(float hdg) { set_headingInDegrees(int16_t(RAD2DEG(hdg))); }

#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
                /** string constructor */
                ParticlePosition(const std::string &names) { from_string(names); }

                /** convert to a string */
                std::string description()
                {
                        std::ostringstream ss;
                        ss << Point2D::description() << "," << headingInDegrees() << "," << confidence()*100;
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
                                        if (getline(iss, token, ','))
                                        {
                                                set_headingInDegrees(int16_t(atoi(token.c_str())));
                                                if (getline(iss, token, ','))
                                                {
                                                        set_confidence(int16_t(atoi(token.c_str())));
                                                }
                                        }
                                }
                        }
                }
#endif
        };

        /**
         * Whiteboard message containing the top 4 particles
         */
        class TopParticles
        {
                ARRAY_PROPERTY(ParticlePosition, particles, NUM_TOP_PARTICLES)  ///< top whiteboard particles
        public:
                /** designated constructor */
                TopParticles() { memset(_particles, 0, sizeof(_particles));
                        for (int i=0; i<NUM_TOP_PARTICLES ; i++) particles(i).set_confidence(-HUGE_VALF); }

                /** copy constructor */
                TopParticles(const TopParticles &other) { *this = other; }

                /** copy assignment operator */
                TopParticles &operator=(const TopParticles &other) { memcpy(_particles, other._particles, sizeof(_particles)); return *this; }

#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
                /** string constructor */
                TopParticles(const std::string &names) { from_string(names); }

                
                /** convert to a string */
                std::string description()
                {
                        std::ostringstream ss;
                        for (int i = 0; i < NUM_TOP_PARTICLES; i++)
                                ss << particles(i).description() << "\t";
                        return ss.str();
                }
                
                /** convert from a string */
                void from_string(const std::string &str)
                {
                        std::istringstream iss(str);
                        std::string token;
                        for (int i = 0; i < NUM_TOP_PARTICLES; i++) if (getline(iss, token, '\t'))
                                particles(i).from_string(token);
                        else break;
                }
#endif
        };
}

#pragma clang diagnostic pop

#endif // PARTICLE_TopPositions_DEFINED
