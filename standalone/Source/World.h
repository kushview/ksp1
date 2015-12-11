#ifndef KSP1_WORLD_H
#define KSP1_WORLD_H

#include "KSP1.h"

namespace KSP1 {

class World
{
public:
    World();
    ~World();

private:
    class Impl;
    ScopedPointer<Impl> impl;
};

}

#endif // KSP1_WORLD_H
