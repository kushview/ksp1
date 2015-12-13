#ifndef CONTROLLERBASE_H
#define CONTROLLERBASE_H

#include "KSP1.h"

namespace KSP1 {

class ControllerBase {
public:
    explicit ControllerBase (ControllerBase* p = nullptr) : parent(p) { }
    virtual ~ControllerBase() { }

private:
    ControllerBase* parent;
    OwnedArray<ControllerBase> children;
};

}
#endif // CONTROLLERBASE_H

