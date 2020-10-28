#ifndef _Ship_
#define _Ship_

#include "../SpaceObject.hpp"
#include "Graphics/Vulkan/VulkanDescriptor.hpp"

#include <vector>
#include <string>

using namespace std;

class ShipAddon;

struct ShipStatusData {
    /* Max Ship Hull */
    float MaxHull;
    /* Max Ship Armor */
    float MaxArmor;
    /* Max Ship Shiled */
    float MaxShiled;

    /* Current Ship Hull */
    float CurrentHull;
    /* Current Ship Armor */
    float CurrentArmor;
    /* Current Ship Shiled */
    float CurrentShiled;
};

struct ShipMovementData {
    /* Current Target*/
    SpaceObject* Target;

    /* Target Orbit Distance */
    float OrbitDistance;
    /* Space Speed */
    float Speed;
    /* Space Acceleration */
    float Acceleration;

    /* Is Orbiting */
    bool IsOrbiting;
    /* Is In Orbit */
    bool InOrbit;
};

struct ShipBattleData {
    /* Is The Ship Currently In Battle*/
    bool Fighting;
};

class Ship : public SpaceObject {
public:
    ShipStatusData shipStatusData;
    ShipMovementData shipMovementData;
    ShipBattleData shipBattleData;

    vector<ShipAddon*> Addons;

    Ship();
    
    /* Process The Ship Orbital Movement*/
    virtual void ProcessOrbit();
    virtual void AddAddon(string Name, Vector3 Position, Quaternion Rotation);

    virtual void Load() override;
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void Clean() override;
};

#endif