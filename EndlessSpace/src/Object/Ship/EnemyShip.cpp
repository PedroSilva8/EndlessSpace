#include "EnemyShip.hpp"
#include "Core/TextureManager.hpp"
#include "Object/Render/MeshRenderer.hpp"
#include "../SpaceManager.hpp"

EnemyShip::EnemyShip() {
    Mode = ENEMY_MODE_IDLE;

    Personality.Agressivenese = ENEMY_AGRESSIVENESE_RETREAT;
    Personality.Profession = ENEMY_PROFESSION_MINER;
    Personality.Retreat = ENEMY_RETREAT_ATTACKED;

    WorkObjectType = "Asteroid";
    Size = 2.0f;
}

void EnemyShip::Load() {
    
    texture = TextureManager::GetTexture("Data/Textures/Test.png");
    LoadObject("Data/Shaders/Tutorial.shader", "Data/Meshes/Main Ship.obj");
    LoadCollider(mesh->mesh);

    Ship::Load();
}

void EnemyShip::Update() {

    switch (Mode) {
        case ENEMY_MODE_IDLE:
            LookForJob();
            break;
        case ENEMY_MODE_FIGHTING:
            ProcessBattle();
            break;
        case ENEMY_MODE_NAVEGATING:
            ProcessMovement();
            break;
        case ENEMY_MODE_RETREAT:
            ProcessMovement();
            break;
        case ENEMY_MODE_WORKING:
            /* Orbit Around Target Or Just Idle Near Target */
            ProcessMovement();
            /* Process Work */
            ProcessWork();
            break;
    }
}

void EnemyShip::LookForJob() {
    vector<SpaceObject*> Targets = vector<SpaceObject*>();

    if (SpaceManager::GetSpaceObjectsOfType(WorkObjectType, Targets)) {
        switch (Personality.Profession) {
            case ENEMY_PROFESSION_SCOUNTER:
                    
                break;
            case ENEMY_PROFESSION_MINER:
                    StartMining(Targets);
                break;
            case ENEMY_PROFESSION_PROTECTOR:

                break;
        }
    }

    Targets.clear();
}

void EnemyShip::ProcessBattle() { 

}

void EnemyShip::ProcessMovement() { 
    switch (Mode) {
        case ENEMY_MODE_WORKING:
            ProcessOrbit();
            break;
    }
}

void EnemyShip::ProcessWork() { 

}

void EnemyShip::StartMining(vector<SpaceObject*> Targets) {
    SpaceObject* Nearest = nullptr;
    float CurDistance = __FLT_MAX__;

    for (SpaceObject* target : Targets) {
        if (target->ObjectType != "Asteroid")
            continue;

        float distance = Distance(target->transform->Position(), transform->Position());
        
        if (distance < CurDistance) {
            CurDistance = distance;
            Nearest = target;
        }
    }

    if (Nearest == nullptr)
        return;

    Target = Nearest;
    Mode = ENEMY_MODE_WORKING; 

    shipMovementData.Target = Target;
    shipMovementData.IsOrbiting = true;
    shipMovementData.InOrbit = false;
}