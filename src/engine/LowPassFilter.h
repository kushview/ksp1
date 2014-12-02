/*
    This file is part of KSP1
    Copyright (C) 2014  Kushview, LLC. All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef KSP1_LOWPASSFILTER_H
#define KSP1_LOWPASSFILTER_H

namespace KSP1 {

    class LowPassFilter
    {
    public:

        LowPassFilter();
        LowPassFilter(const LowPassFilter& lpf);

        float getCutoff() const             { return cutoff; }
        float getResonance() const          { return resonance; }
        void setCutoff (float c)       { cutoff = c; }
        void setResonance (float r) { resonance = r; }

        /** Pass a value through the filter updating the buffer values. */
        inline void processMono (float* sample, int count)
        {
            while (true)
            {
                bandPassBuffer_L = resonance * bandPassBuffer_L + cutoff
                        * ((*sample) - lowPassBuffer_L);

                lowPassBuffer_L += cutoff * bandPassBuffer_L;

                *sample = lowPassBuffer_L;

                count--;
                if (0 == count)
                    break;

                sample++;
            }
        }

        inline void processStereo (float* sampleL, float* sampleR, int count)
        {
            int c = count;
            while (1)
            {
                bandPassBuffer_L = resonance * bandPassBuffer_L + cutoff
                        * ((*sampleL) - lowPassBuffer_L);

                lowPassBuffer_L += cutoff * bandPassBuffer_L;

                *sampleL = lowPassBuffer_L;

                c--;
                if (0 == c)
                    break;

                sampleL++;
            }

            c = count;
            while (1)
            {
                bandPassBuffer_R = resonance * bandPassBuffer_R + cutoff
                        * ((*sampleR) - lowPassBuffer_R);

                lowPassBuffer_R += cutoff * bandPassBuffer_R;

                *sampleR = lowPassBuffer_R;

                c--;
                if (0 == c)
                    break;

                sampleR++;
            }
        }

        /** Reset the filter buffers to zero (0.0f) */
        void resetBuffers();

    private:
        float cutoff;               ///< Filter cutoff (0..1)
        float resonance;            ///< Filter resonant frequency (0..1)
        float lowPassBuffer_L;		///< Low pass filter buffer
        float lowPassBuffer_R;		///< Low pass filter buffer
        float bandPassBuffer_L;		///< Band pass filter buffer
        float bandPassBuffer_R;		///< Band pass filter buffer
    };

} /* namespace KSP1 */

#endif
