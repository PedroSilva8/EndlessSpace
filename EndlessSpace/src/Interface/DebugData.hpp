#ifndef _DEBUGDATA_
#define _DEBUGDATA_

#include "InterfaceObject/InterfaceObject.hpp"

class DebugData : public InterfaceObject {
public:
    void Render() override;
};

#endif