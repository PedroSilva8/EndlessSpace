#ifndef _PLAYERSHIP_
#define _PLAYERSHIP_

#include "Ship.hpp"

class PlayerShip : public Ship {
public:
    Mesh* movementMesh;

    PlayerShip();

    void Load() override;
    void Update() override;
    void OnCollision(RayTestResult result) override;
};

#endif