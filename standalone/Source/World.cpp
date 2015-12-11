#include "KSP1.h"
#include "World.h"

namespace KSP1 {
class World::Impl {
public:
};

World::World()
{
    impl = new Impl();
}

World::~World()
{
    impl = nullptr;
}
}
