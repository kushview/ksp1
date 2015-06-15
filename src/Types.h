/*
    Types.h - This file is part of KSP1

    Copyright (C) 2013 Kushview, LLC  All rights reserved.
      * Michael Fisher <mfisher@kushview.net>
*/

#ifndef KSP1_SAMPLER_TYPES_H
#define KSP1_SAMPLER_TYPES_H

#include "KSP1.h"

namespace KSP1
{
    typedef AtomicValue<int>    AtomicInt;
    typedef AtomicLock          DataLock;
    typedef AtomicValue<double> AtomicDouble;
    typedef AtomicValue<bool>   AtomicFlag;
    typedef AtomicValue<float>  AtomicFloat;
    typedef AtomicValue<int64>  AtomicFrame;
}

namespace Tags
{
    using namespace Element::Slugs;
    static const Identifier articulation    = "articulation";
    static const Identifier interval        = "interval";
    static const Identifier key             = "key";
    static const Identifier layer           = "layer";
    static const Identifier legato          = "legato";
    static const Identifier panning         = "panning";
    static const Identifier session         = "session";
    static const Identifier strokeType      = "strokeType";
    static const Identifier triggerMode     = "triggerMode";
    static const Identifier voiceGroup      = "voiceGroup";
    static const Identifier velocityUpper   = "velocityUpper";
    static const Identifier velocityLower   = "velocityLower";
    static const Identifier resonance       = "resonance";
    static const Identifier cutoff          = "cutoff";
}


#endif  // TYPES_H_INCLUDED
