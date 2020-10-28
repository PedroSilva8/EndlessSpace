#ifndef _PLAYERSHIP_
#define _PLAYERSHIP_

#include "Ship.hpp"

class PieMenu;

class PlayerShip : public Ship {
public:

    SpaceObject* target;
    PieMenu* menu;

    PlayerShip();
    
    void Load() override;
    void Update() override;
    void OnCollision(RayTestResult result) override;
};

#endif