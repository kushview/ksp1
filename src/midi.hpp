#pragma once

#include <cstdint>

namespace ksp1 {
namespace gm {
    enum class drums: uint8_t {
        HighQ = 27,
        Slap,
        ScratchPush,
        ScratchPull,
        DrumkSticks,
        SquareClick,
        MetronomeClick,
        MetronomeBell,

        AcousticBasDrum,
        ElectricBassDrum,
        SideStick,
        AcousticSnare,
        HandClap,
        ElectricSnare,
        LowFloorTom,
        ClosedHiHat,
        HighFloorTom,
        PedalHiHat,
        LowTom,
        OpenHiHat,
        LowMidTom,
        HighMidTom,
        CrashCymbal_1,
        HighTom,
        RideCymbal_1,
        ChineseCymbal,
        RideBell,
        Tambourine,
        SplashCymbal,
        Cowbell,
        CrashCymbal_2,
        Vibraslap,
        RideCymbal_2,
        HighBongo,
        LowBongo,
        MuteHighConga,
        OpenHighConga,
        LowConga,
        HighTimbale,
        LowTimable,
        HighAgogo,
        LowAgogo,
        Cabasa,
        Maracas,
        ShortWhistle,
        LongWhistle,
        ShortGuiro,
        LongGuiro,
        Claves,
        HighWoodBlock,
        LowWoodbBock,
        MuteCuica,
        OpenCuica,
        MuteTriangle,
        OpenTriangle,

        Shaker,
        JingleBell,
        Belltree,
        Castanets,
        MuteSurdo,
        OpenSurdo
    }
}

}
