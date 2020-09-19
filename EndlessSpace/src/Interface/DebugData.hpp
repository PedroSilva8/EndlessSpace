#ifndef _DEBUGDATA_
#define _DEBUGDATA_

#include "InterfaceObject/InterfaceObject.hpp"

class DebugData : public InterfaceObject {
public:
    float TotalTime = 0.0f;
    float Frames = 0.0f;
    float FrameRate = 0.0f;
    
    void Load() override;
    void PreRender() override;
    void Render() override;
    void PosRender() override;
};

#endif