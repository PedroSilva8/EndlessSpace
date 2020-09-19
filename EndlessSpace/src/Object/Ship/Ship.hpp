#ifndef _Ship_
#define _Ship_

#include "../SpaceObject.hpp"
#include "Graphics/Vulkan/VulkanDescriptor.hpp"

class Ship : public SpaceObject {
public:
    Vector3 MovementTarget;
    float orbitDistance;
    float speed;
    float acceleration;
    bool isOrbiting;
    bool inOrbit;
    
    virtual void Load() override;
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void Clean() override;
};

#endif