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

#ifndef KSP1_ADSR_H
#define KSP1_ADSR_H

#include <iostream>

namespace KSP1 {

    class ADSR
    {
    public:

        enum State
        {
            Attack,
            Decay,
            Sustain,
            Release,
            Idle
        };

        ADSR (float a = 0.0, float d = 0.0,
              float s = 0.8, float r = 1000);

        ADSR (const ADSR& orig);
        ADSR& operator= (const ADSR& other);
        ~ADSR();

        inline float attack()  const { return attackTime; }
        inline float decay()   const { return decayTime; }
        inline float sustain() const { return sustainLevel; }
        inline float release() const { return releaseTime; }

        /** Set the attack time */
        inline void
        setAttack (float attack)
        {
            attackTime = attack;
            if (attack != 0.0f)
                invAttack = 1.0f / attack;
            else
                invAttack = 1.0f;
        }

        /** Set the decay time */
        inline void
        setDecay (float decay)
        {
            decayTime = decay;
            if (decay != 0.0f)
                invDecay = 1.0f / decay;
            else
                invDecay = 1.0f;
        }

        /** Set the sustain level */
        inline void
        setSustain (float sustain)
        {
            sustainLevel = sustain;
        }

        /** Set the release time */
        inline void
        setRelease (float release)
        {
            releaseTime = release;

            if (release != 0.0f)
                invRelease = 1.0f / release;
            else
                invRelease = 1.0f;
        }

        /** Obsolete */
        inline float getValue (float step);

        /** Fill an array with ADSR values */
        inline void processBuffer (int count, float Data[]);

        inline float setFutureRelease (float when = 0.0f);
        inline void forceRelease (float max);

        /** Reset Tick positions */
        inline void reset();

        /** Get the current ADSR state, this is only valid during processing */
        inline int state() const { return adsrState; }

        /** Copy ADSR values plus tick position(s) */
        void copy (const ADSR& other);

        /** Copy ADSR values only

            @param other The other ADSR to sync from
            @param resetTicks  If true, tick positions are reset
        */
        void sync (const ADSR& other, bool resetTicks);

        /** Compares ADSR values only. Tick positions are ignored. This is
            so the ADSR can be processed my multiple sources. @see ADSR::sync */
        inline bool
        operator== (const ADSR& other)
        {
            return attackTime == other.attackTime &&
                    decayTime == other.decayTime &&
                    sustainLevel == other.sustainLevel &&
                    releaseTime == other.releaseTime;
        }

        inline bool operator!= (const ADSR& other) {
            return ! (this->operator== (other));
        }

    private:

        float attackTime;    ///< Attack time (Unit depends on context)
        float decayTime;     ///< Decay time (Unit depends on context)
        float sustainLevel;  ///< Sustain level (ratio 0.0 - 1.0)
        float releaseTime;   ///< Release time (Unit depends on context)

        float invAttack;     ///< cached inverse attack
        float invDecay;      ///< cached inverse decay
        float invRelease;    ///< cached inverse release

        State adsrState;     ///< Current ADSR state
        float adsrValue;     ///< Current ADSR value

        float releaseValue;  ///< Release value
        float futureRelease; ///< Release phase will be entered when totalTicks >= this value.

        float modeTicks;     ///< This counts the number of ticks in each mode of the ADSR.
        float totalTicks;    ///< The number of ticks from the beginning, regardless of mode.

    };

    inline void
    ADSR::processBuffer (int count, float buffer[])
    {
        short ticks;
        short duration;
        short future = count + 1;
        short iMax;

        ticks = static_cast<short> (modeTicks);
        if (futureRelease != 0.0f)
            if (count + totalTicks >= futureRelease)
                future = static_cast<short> (futureRelease - (count + totalTicks));

        for (int i = 0; i < count; i++)
        {
            switch (adsrState)
            {
                case Attack:
                {
                    duration = static_cast<short> (attackTime);
                    for(; ticks < duration && i < count && i < future; i++)
                    {
                        adsrValue = ticks * invAttack;
                        ticks++;
                        buffer[i] = adsrValue;
                    }
                    if (i >= future)
                    {
                        adsrState = Release;
                        futureRelease = 0.0f;
                        releaseValue = adsrValue;
                        ticks = 0;
                    }
                    else if (ticks >= duration)
                    {
                        adsrState = Decay;
                        adsrValue = 1.0f;
                        ticks = 0;
                    }
                    break;
                }
                case Decay:
                {
                    duration = static_cast<short> (decayTime);
                    for(; ticks < duration && i < count && i < future; i++)
                    {
                        adsrValue = 1.0f - (ticks * invDecay) * (1.0f - sustainLevel);
                        ticks++;
                        buffer[i] = adsrValue;
                    }
                    if (i >= future)
                    {
                        adsrState = Release;
                        futureRelease = 0.0f;
                        releaseValue = adsrValue;
                        ticks = 0;
                    }
                    else if (ticks >= duration)
                    {
                        adsrState = Sustain;
                        adsrValue = sustainLevel;
                        ticks = 0;
                    }
                    break;
                }
                case Sustain:
                {
                    iMax = (count < future ? count : future);
                    for(; i < iMax; i++)
                    {
                        buffer[i] = sustainLevel;
                    }
                    if (i >= future)
                    {
                        adsrState = Release;
                        futureRelease = 0.0f;
                        releaseValue = adsrValue;
                        ticks = 0;
                    }
                    break;
                }
                case Release:
                {
                    duration = static_cast<short> (releaseTime);
                    for(; ticks < duration && i < count; i++)
                    {
                        adsrValue = (1.0f - ticks * invRelease) * releaseValue;
                        ticks++;
                        buffer[i] = adsrValue;
                    }
                    if (ticks >= duration)
                    {
                        adsrState = Idle;
                        adsrValue = 1.0f;
                        ticks = 0;
                    }
                    break;
                }
                case Idle:
                default:
                {
                    // Fill with zeros to end
                    for(; i < count; i++)
                        buffer [i] = 0.0f;
                    ticks = 0;
                }
            }
        }

        totalTicks += count;
        modeTicks = static_cast<float> (ticks);
    }

    inline float
    ADSR::setFutureRelease (float when)
    {
        if (adsrState == Idle ) {
            return 0.0f;
        }

        if (0.0f == when)
        {
            if (adsrState != Release)
            {
                releaseValue = adsrValue;
                adsrState = Release;
                modeTicks = 0.f;

                return releaseValue;
            }

            return 1.0f;
        }
        else
        {
            if (0.0f == futureRelease)
                futureRelease = totalTicks + when;

            return adsrValue;
        }
    }

    inline void
    ADSR::forceRelease (float maxRelease)
    {
        if( adsrState == Idle)
            return;

        if( adsrState != Release )
        {
            releaseValue = adsrValue;
            adsrState = Release;
            modeTicks = 0.0f;
        }

        if (maxRelease < releaseTime && maxRelease > 0.0 )
        {
            setRelease (maxRelease);
            releaseValue = adsrValue;  // Avoid discontinuity
            modeTicks = 0.0f;
        }
    }

    inline void
    ADSR::reset()
    {
        adsrState = Attack;
        adsrValue = 0.0f;
        modeTicks = 0.0f;
        totalTicks = 0.0f;
        futureRelease = 0.0f;
    }
}

#endif
