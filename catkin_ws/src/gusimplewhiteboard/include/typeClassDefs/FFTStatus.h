/*
 *  FFTStatus.h
 *  gusimplewhiteboard
 *
 *  reated by Rene Hexel on 24/06/2014.
 *  Copyright (c) 2014 Rene Hexel. All rights reserved.
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
#ifndef FFTStatus_DEFINED
#define FFTStatus_DEFINED

#include <cassert>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
#include <sstream>
#include <gu_util.h>
#endif
#include "gusimplewhiteboard.h"
#include "wb_fft_frequencies.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winvalid-offsetof"

namespace guWhiteboard
{
    /**
     * Container for RMS levels and dominant frequencies
     */
    class FFTStatus: public fft_dominant_frequency
    {
    public:
        /** designated constructor */
        FFTStatus(int16_t lrms, int16_t rrms, uint16_t hi, uint16_t lo, uint16_t fsk, va_list freqs): fft_dominant_frequency(lrms, rrms, hi, lo, fsk, freqs)
        {
            if (!freqs) memset(static_cast<void *>(_frequencies), 0, GU_SIMPLE_WHITEBOARD_BUFSIZE - offsetof(FFTStatus, _frequencies));
        }

        /** alternate constructor */
        FFTStatus(int16_t lrms = 0, int16_t rrms = 0, uint16_t hi = FSK_DEFAULT_HI, uint16_t lo = FSK_DEFAULT_LO, uint16_t fsk = 0, ...): fft_dominant_frequency(lrms, rrms, hi, lo, fsk, 0)
        {
            memset(static_cast<void *>(_frequencies), 0, GU_SIMPLE_WHITEBOARD_BUFSIZE - offsetof(FFTStatus, _frequencies));

            if (!fsk) return;

            va_list freqs;
            va_start(freqs, fsk);
            int16_t freq = static_cast<int16_t>(va_arg(freqs, int));
            int16_t *freqp = &_frequencies->left();
            while (freq) { *freqp++ = freq; freq = static_cast<int16_t>(va_arg(freqs, int)); }
            va_end(freqs);
        }

        /** copy constructor */
        FFTStatus(const FFTStatus &other) { memcpy(static_cast<void *>(this), static_cast<const void *>(&other), GU_SIMPLE_WHITEBOARD_BUFSIZE); }

        /** copy assignment operator */
        FFTStatus &operator=(const FFTStatus &other) { memcpy(static_cast<void *>(this), static_cast<const void *>(&other), GU_SIMPLE_WHITEBOARD_BUFSIZE); return *this; }

        /** number of frequencies that can be put on the wb */
        static int num_frequencies() { const int n = (GU_SIMPLE_WHITEBOARD_BUFSIZE - offsetof(FFTStatus, _frequencies)) / sizeof(fft_frequency_level_pair); assert(n == FFT_DOMINANT_NUMFREQ); return n; }

#ifdef WHITEBOARD_POSTER_STRING_CONVERSION
        /** string constructor */
        FFTStatus(const std::string &names) { from_string(names); }

        /** const char *constructor */
        FFTStatus(const char *names) { from_string(std::string(names)); }

        /** convert to a string */
        std::string description()
        {
            using namespace std;
            using namespace guWhiteboard;

            ostringstream ss;
            ss << rms().left() << "/" << rms().right() << ", ";
            ss << fsk().hi_freq() << ":" << fsk().lo_freq() << "@" << fsk().hi_percentage();
            unsigned n = FFTStatus::num_frequencies();
            for (unsigned i = 0; i < n; i++)
            {
                const fft_frequency_level_pair &f = frequencies(i);
                if (!f.left() && !f.right())
                    break;
                ss << ", " << f.left() << "+" << f.right();
            }
            return ss.str();
        }

        /** convert from a string */
        void from_string(const std::string &str)
        {
            using namespace std;
            using namespace guWhiteboard;
            vector<string> components = components_of_string_separated(str, ',',  /*trim*/ true);
            size_t n = components.size();
            if (!n) return;

            /*
             * get RMS left + right values
             */
            vector<string> rmsvals = components_of_string_separated(components[0], '/',  /*trim*/ true);
            size_t k = rmsvals.size();
            if (!k) return;
            rms().set_left(static_cast<int16_t>(atoi(rmsvals[0].c_str())));
            if (k < 2) return;
            rms().set_right(static_cast<int16_t>(atoi(rmsvals[1].c_str())));

            /*
             * get FSK high and low frequencies and percentage values
             */
            if (n < 2) return;
            vector<string> fskvals = components_of_string_separated(components[1], ':',  /*trim*/ true);
            k = rmsvals.size();
            if (!k) return;
            fsk().set_hi_freq(static_cast<int16_t>(atoi(fskvals[0].c_str())));
            if (k < 2) return;
            fsk().set_lo_freq(static_cast<int16_t>(atoi(fskvals[1].c_str())));
            vector<string> percentvals = components_of_string_separated(fskvals[1], '@', /*trim*/ true);
            k = percentvals.size();
            if (k > 1) fsk().set_hi_percentage(static_cast<int16_t>(atoi(percentvals[1].c_str())));

            /*
             * get dominant frequencies and their levels
             */
            for (unsigned i = 2; i < n; i++)
            {
                vector<string> freqvals = components_of_string_separated(components[i], '+',  /*trim*/ true);
                k = freqvals.size();
                if (!k) break;
                fft_frequency_level_pair &frequency = frequencies(i-2);
                frequency.set_left(static_cast<int16_t>(atoi(freqvals[0].c_str())));
                if (k > 1) frequency.set_right(static_cast<int16_t>(atoi(freqvals[1].c_str())));
                if (!frequency.left() && !frequency.right())
                    break;              // if both are zero, we are done
            }
        }
#endif // WHITEBOARD_POSTER_STRING_CONVERSION
    };
}

#pragma clang diagnostic pop

#endif // FFTStatus_DEFINED
