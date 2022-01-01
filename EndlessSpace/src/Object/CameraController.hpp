#ifndef _CAMERA_CONTROLLER_
#define _CAMERA_CONTROLLER_

#include "Object/Component.hpp"

class CameraController : public Component {
public:
    void Load() override;
    void Update() override;
};

#endif