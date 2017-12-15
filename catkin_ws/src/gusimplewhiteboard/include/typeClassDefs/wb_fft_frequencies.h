/*
 *  fft_frequencies.h
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
#ifndef _wb_fft_frequencies_h_
#define _wb_fft_frequencies_h_

#ifdef __cplusplus
#include <cstdarg>
#endif

#include <gu_util.h>

#ifdef GU_SIMPLE_WHITEBOARD_BUFSIZE
#define FFT_DOMINANT_NUMFREQ    ((GU_SIMPLE_WHITEBOARD_BUFSIZE - sizeof(struct rms_strength) - sizeof(struct fsk_frequencies)) / sizeof(fft_frequency_level_pair))
#else
#define FFT_DOMINANT_NUMFREQ    0   // no wb -> don't register size
#endif

#define FSK_DEFAULT_HI  320     ///< default hi frequency for fsk
#define FSK_DEFAULT_LO  200     ///< default lo frequency for fsk

struct fft_frequency_level_pair ///< one frequency pair for a stereo channels
{
    PROPERTY(int16_t, left)     ///< left frequency in Hz
    PROPERTY(int16_t, right)    ///< right frequency in Hz

#ifdef __cplusplus
    fft_frequency_level_pair(int16_t l = 0, int16_t r = 0): _left(l), _right(r) {}
#endif
};

struct rms_strength             ///< RMS levels for a stereo channel pair
{
    PROPERTY(int16_t, left)     ///< RMS level for the left channel
    PROPERTY(int16_t, right)    ///< RMS level for the right channel

#ifdef __cplusplus
    rms_strength(int16_t l = 0, int16_t r = 0): _left(l), _right(r) {}
#endif
};


struct fsk_frequencies          ///< FSK frequency pair and keying values
{
    unsigned _hi_freq: 12;      ///< high frequency (0..8191 Hz) divided by 2
    unsigned _lo_freq: 12;      ///< low frequency (0..8191 Hz) divided by 2
    unsigned _hi_percentage: 7; ///< FSK hi percentage (0..100 %)
    unsigned _unused_pad: 1;    ///< reserved for future use

#ifdef __cplusplus
    fsk_frequencies(uint16_t hi = FSK_DEFAULT_HI, uint16_t lo = FSK_DEFAULT_LO, uint16_t fsk = 0): _hi_freq(hi/2), _lo_freq(lo/2), _hi_percentage(fsk), _unused_pad(0) {}
    uint16_t hi_freq() const { return _hi_freq * 2; }
    uint16_t lo_freq() const { return _lo_freq * 2; }
    uint16_t hi_percentage() const { return _hi_percentage; }
    float hi_ratio() const { return static_cast<float>(_hi_percentage) / 100.f; }
    void set_hi_freq(uint16_t hi = FSK_DEFAULT_HI) { _hi_freq = hi/2; }
    void set_lo_freq(uint16_t lo = FSK_DEFAULT_LO) { _lo_freq = lo/2; }
    void set_hi_percentage(uint16_t percent = 0) { _hi_percentage = percent; }
    void set_hi_ratio(float r) { _hi_percentage = static_cast<unsigned>(100.f * r); }
#endif
};


struct fft_dominant_frequency   ///< A list of dominant frequencies (top to bottom)
{
    PROPERTY(struct rms_strength, rms)      ///< overall RMS levels
    PROPERTY(struct fsk_frequencies, fsk)   ///< FSK frequency comparison percentage
    ARRAY_PROPERTY(struct fft_frequency_level_pair, frequencies, FFT_DOMINANT_NUMFREQ)   ///< frequency levels

#ifdef __cplusplus
    fft_dominant_frequency(int16_t lrms, int16_t rrms, uint16_t hi, uint16_t lo, uint16_t fsk, va_list freqs): _rms(lrms, rrms), _fsk(hi, lo, fsk)
    {
        if (freqs)
        {
            int16_t *freqp = &_frequencies->left();
            int16_t freq = static_cast<int16_t>(va_arg(freqs, int));
            while (freq) { *freqp++ = freq; freq = static_cast<int16_t>(va_arg(freqs, int)); }
        }
    }
    fft_dominant_frequency(int16_t lrms = 0, int16_t rrms = 0, uint16_t hi = FSK_DEFAULT_HI, uint16_t lo = FSK_DEFAULT_LO, uint16_t fsk = 0, ...): _rms(lrms, rrms), _fsk(hi, lo, fsk)
    {
        if (!rrms) return;

        va_list freqs;
        va_start(freqs, fsk);
        int16_t freq = static_cast<int16_t>(va_arg(freqs, int));
        int16_t *freqp = &_frequencies->left();
        while (freq) { *freqp++ = freq; freq = static_cast<int16_t>(va_arg(freqs, int)); }
        va_end(freqs);
    }
#endif
};

#endif //#_wb_fft_frequencies_h_
